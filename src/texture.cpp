/*
 * Copyright © 2008 Ben Smith
 * Copyright © 2010-2011 Linaro Limited
 *
 * This file is part of the glmark2 OpenGL (ES) 2.0 benchmark.
 *
 * glmark2 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * glmark2 is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * glmark2.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *  Ben Smith (original glmark benchmark)
 *  Alexandros Frantzis (glmark2)
 */
#include "texture.h"

static void
setup_texture(GLuint *tex, GLenum format, SDL_Surface *surface,
              GLint min_filter, GLint mag_filter)
{
    glGenTextures(1, tex);
    glBindTexture(GL_TEXTURE_2D, *tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
    glTexImage2D(GL_TEXTURE_2D, 0, format, surface->w, surface->h, 0,
                 format, GL_UNSIGNED_BYTE, surface->pixels);

    if ((min_filter != GL_NEAREST && min_filter != GL_LINEAR) ||
        (mag_filter != GL_NEAREST && mag_filter != GL_LINEAR))
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

int
Texture::load(const char pFilename[], GLuint *pTexture, ...)
{
    SDL_Surface *surface;
    GLenum texture_format = GL_RGB;
    GLint  nOfColors;

    if ((surface = SDL_LoadBMP(pFilename))) {
        if ((surface->w & (surface->w - 1)) != 0)
            printf("warning: image.bmp's width is not a power of 2\n");

        if ((surface->h & (surface->h - 1)) != 0)
            printf("warning: image.bmp's height is not a power of 2\n");

        nOfColors = surface->format->BytesPerPixel;
        if (nOfColors == 4) {
            texture_format = GL_RGBA;
            // If the picture is not RGBA convert it
            if (surface->format->Rmask != 0x000000ff) {
                SDL_PixelFormat format = {
                    surface->format->palette,
                    surface->format->BitsPerPixel,
                    surface->format->BytesPerPixel,
                    surface->format->Rloss, surface->format->Gloss,
                    surface->format->Bloss, surface->format->Aloss,
                    0,  8, 16, 24,
                    0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000,
                    surface->format->colorkey, surface->format->alpha
                };
                SDL_Surface *tmp = SDL_ConvertSurface(surface, &format, 0);
                SDL_FreeSurface(surface);
                surface = tmp;
            }
        }
        else if (nOfColors == 3) {
            texture_format = GL_RGB;
            // If the picture is not RGB convert it
            if (surface->format->Rmask != 0x000000ff) {
                SDL_PixelFormat format = {
                    surface->format->palette,
                    surface->format->BitsPerPixel,
                    surface->format->BytesPerPixel,
                    surface->format->Rloss, surface->format->Gloss,
                    surface->format->Bloss, surface->format->Aloss,
                    0,  8, 16, 24,
                    0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000,
                    surface->format->colorkey, surface->format->alpha
                };
                SDL_Surface *tmp = SDL_ConvertSurface(surface, &format, 0);
                SDL_FreeSurface(surface);
                surface = tmp;
            }
        }
        else {
            printf("warning: the image is not truecolor..  this will probably break\n");
        }

        va_list ap;
        va_start(ap, pTexture);
        GLint arg;

        while ((arg = va_arg(ap, GLint)) != 0) {
            GLint arg2 = va_arg(ap, GLint);
            setup_texture(pTexture, texture_format, surface, arg, arg2);
            pTexture++;
        }

        va_end(ap);
    }
    else {
        fprintf(stderr, "SDL could not load image.bmp: %s\n", SDL_GetError());
        return 0;
    }

    if (surface)
        SDL_FreeSurface(surface);

    return 1;
}
