
#include "loadimage.h"
GLuint loadTexture(string filenameString, GLenum minificationFilter, GLenum magnificationFilter)
{
    // Get the filename as a pointer to a const char array to play nice with FreeImage
    const char* filename = filenameString.c_str();
    
    // Determine the format of the image.
    // Note: The second paramter ('size') is currently unused, and we should use 0 for it.
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename , 0);
    
    // Image not found? Abort! Without this section we get a 0 by 0 image with 0 bits-per-pixel but we don't abort, which
    // you might find preferable to dumping the user back to the desktop.
    if (format == -1)
    {
      //  cout << "Could not find image: " << filenameString << " - Aborting." << endl;
        exit(-1);
    }
    
    // Found image, but couldn't determine the file format? Try again...
    if (format == FIF_UNKNOWN)
    {
      //  cout << "Couldn't determine file format - attempting to get from file extension..." << endl;
        
        // ...by getting the filetype from the filename extension (i.e. .PNG, .GIF etc.)
        // Note: This is slower and more error-prone that getting it from the file itself,
        // also, we can't use the 'U' (unicode) variant of this method as that's Windows only.
        format = FreeImage_GetFIFFromFilename(filename);
        
        // Check that the plugin has reading capabilities for this format (if it's FIF_UNKNOWN,
        // for example, then it won't have) - if we can't read the file, then we bail out =(
        if ( !FreeImage_FIFSupportsReading(format) )
        {
       //     cout << "Detected image format cannot be read!" << endl;
            exit(-1);
        }
    }
    
    // If we're here we have a known image format, so load the image into a bitap
    FIBITMAP* bitmap = FreeImage_Load(format, filename);
    
    // How many bits-per-pixel is the source image?
    int bitsPerPixel =  FreeImage_GetBPP(bitmap);
    
    // Convert our image up to 32 bits (8 bits per channel, Red/Green/Blue/Alpha) -
    // but only if the image is not already 32 bits (i.e. 8 bits per channel).
    // Note: ConvertTo32Bits returns a CLONE of the image data - so if we
    // allocate this back to itself without using our bitmap32 intermediate
    // we will LEAK the original bitmap data, and valgrind will show things like this:
    //
    // LEAK SUMMARY:
    //  definitely lost: 24 bytes in 2 blocks
    //  indirectly lost: 1,024,874 bytes in 14 blocks    <--- Ouch.
    //
    // Using our intermediate and cleaning up the initial bitmap data we get:
    //
    // LEAK SUMMARY:
    //  definitely lost: 16 bytes in 1 blocks
    //  indirectly lost: 176 bytes in 4 blocks
    //
    // All above leaks (192 bytes) are caused by XGetDefault (in /usr/lib/libX11.so.6.3.0) - we have no control over this.
    //
    FIBITMAP* bitmap32;
    if (bitsPerPixel == 32)
    {
     //   cout << "Source image has " << bitsPerPixel << " bits per pixel. Skipping conversion." << endl;
        bitmap32 = bitmap;
    }
    else
    {
       // cout << "Source image has " << bitsPerPixel << " bits per pixel. Converting to 32-bit colour." << endl;
        bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
    }
    
    // Some basic image info - strip it out if you don't care
    int imageWidth  = FreeImage_GetWidth(bitmap32);
    int imageHeight = FreeImage_GetHeight(bitmap32);
   // cout << "Image: " << filenameString << " is size: " << imageWidth << "x" << imageHeight << "." << endl;
    
    // Get a pointer to the texture data as an array of unsigned bytes.
    // Note: At this point bitmap32 ALWAYS holds a 32-bit colour version of our image - so we get our data from that.
    // Also, we don't need to delete or delete[] this textureData because it's not on the heap (so attempting to do
    // so will cause a crash) - just let it go out of scope and the memory will be returned to the stack.
    GLubyte* textureData = FreeImage_GetBits(bitmap32);
    
    // Generate a texture ID and bind to it
    GLuint tempTextureID;
    glGenTextures(1, &tempTextureID);
    glBindTexture(GL_TEXTURE_2D, tempTextureID);
    
    // Construct the texture.
    // Note: The 'Data format' is the format of the image data as provided by the image library. FreeImage decodes images into
    // BGR/BGRA format, but we want to work with it in the more common RGBA format, so we specify the 'Internal format' as such.
    glTexImage2D(GL_TEXTURE_2D,    // Type of texture
                 0,                // Mipmap level (0 being the top level i.e. full size)
                 GL_RGBA,          // Internal format
                 imageWidth,       // Width of the texture
                 imageHeight,      // Height of the texture,
                 0,                // Border in pixels
                 GL_BGRA,          // Data format
                 GL_UNSIGNED_BYTE, // Type of texture data
                 textureData);     // The image data to use for this texture
    
    // Specify our minification and magnification filters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minificationFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnificationFilter);
    
    // If we're using MipMaps, then we'll generate them here.
    // Note: The glGenerateMipmap call requires OpenGL 3.0 as a minimum.
    if (minificationFilter == GL_LINEAR_MIPMAP_LINEAR   ||
        minificationFilter == GL_LINEAR_MIPMAP_NEAREST  ||
        minificationFilter == GL_NEAREST_MIPMAP_LINEAR  ||
        minificationFilter == GL_NEAREST_MIPMAP_NEAREST)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    // Check for OpenGL texture creation errors
//    GLenum glError = glGetError();
//    if(glError)
//    {
//        cout << "There was an error loading the texture: "<< filenameString << endl;
//        
//        switch (glError)
//        {
//            case GL_INVALID_ENUM:
//                cout << "Invalid enum." << endl;
//                break;
//                
//            case GL_INVALID_VALUE:
//                cout << "Invalid value." << endl;
//                break;
//                
//            case GL_INVALID_OPERATION:
//                cout << "Invalid operation." << endl;
//                
//            default:
//                cout << "Unrecognised GLenum." << endl;
//                break;
//        }
//        
//        cout << "See https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml for further details." << endl;
//    }
    
    // Unload the 32-bit colour bitmap
    FreeImage_Unload(bitmap32);
    
    // If we had to do a conversion to 32-bit colour, then unload the original
    // non-32-bit-colour version of the image data too. Otherwise, bitmap32 and
    // bitmap point at the same data, and that data's already been free'd, so
    // don't attempt to free it again! (or we'll crash).
    if (bitsPerPixel != 32)
    {
        FreeImage_Unload(bitmap);
    }
    
    // Finally, return the texture ID
    return tempTextureID;
}


void drawImg(double x0,double y0,double x1,double y1, bool r, GLuint textureID)
{
    
    /* assign it a reference. You can use any number */
    //glGenTextures(1, &gimp_image);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    /* enable texturing. If you don't do this, you won't get any image displayed */
    glEnable(GL_TEXTURE_2D);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    /* draw the texture to the screen, on a white box from (0,0) to (1, 1). Other shapes may be used. */
    glColor3f(1.0, 1.0, 1.0);
    
    double vv = 1.0;
    if(r) vv = 0.0;
    /* you need to put a glTexCoord and glVertex call , one after the other, for each point */
    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 1 - vv);	glVertex2d(x0, y0);
    glTexCoord2d(0.0, vv);	glVertex2d(x0, y1);
    glTexCoord2d(1.0, vv);	glVertex2d(x1, y1);
    glTexCoord2d(1.0, 1 - vv);	glVertex2d(x1, y0);
    glEnd();
    glDisable(GL_BLEND);
    
    /* clean up */
    glDisable(GL_TEXTURE_2D);
    //    glDeleteTextures(1, &logo);
}


vector<GLuint> loadAllImg()
{
    vector<string> paths =
    {
        "/Users/KhalidRamadan/Graphics/OpenGL/OpenGL/Images/room/dark_closed.png",
        "/Users/KhalidRamadan/Graphics/OpenGL/OpenGL/Images/room/light_closed.png",
        "/Users/KhalidRamadan/Graphics/OpenGL/OpenGL/Images/room/dark_open.png",
        "/Users/KhalidRamadan/Graphics/OpenGL/OpenGL/Images/room/light_open.png",
        "/Users/KhalidRamadan/Graphics/OpenGL/OpenGL/Images/color.png",
        "/Users/KhalidRamadan/Graphics/OpenGL/OpenGL/Images/police/police.png",
        "/Users/KhalidRamadan/Graphics/OpenGL/OpenGL/Images/prisoners/prisoner0.png",
        "/Users/KhalidRamadan/Graphics/OpenGL/OpenGL/Images/prisoners/prisoner1.png",
        "/Users/KhalidRamadan/Graphics/OpenGL/OpenGL/Images/prisoners/prisoner2.png",
        "/Users/KhalidRamadan/Graphics/OpenGL/OpenGL/Images/prisoners/prisoner3.png",
        "/Users/KhalidRamadan/Graphics/OpenGL/OpenGL/Images/prisoners/prisoner4.png",
        "/Users/KhalidRamadan/Graphics/OpenGL/OpenGL/Images/prisoners/prisoner5.png",
        "/Users/KhalidRamadan/Graphics/OpenGL/OpenGL/Images/prisoners/prisoner6.png",
        "/Users/KhalidRamadan/Graphics/OpenGL/OpenGL/Images/prisoners/prisoner7.png",
        "/Users/KhalidRamadan/Graphics/OpenGL/OpenGL/Images/prisoners/prisoner8.png",
        "/Users/KhalidRamadan/Graphics/OpenGL/OpenGL/Images/prisoners/prisoner9.png",
    };
    vector<GLuint> imageids;
    
    imageids.resize(paths.size());
    for(int i = 0; i < (int)paths.size(); i++)
        imageids[i] = loadTexture(paths[i]);
    
    return imageids;
}
