#include "GlobalFuncs.h"
#include "../Vendor/stb_image.h"

bool Key(const char& key){
    return GetAsyncKeyState(int(key)) & 0x8000;
}

void SetUpTex(const SetUpTexsParams& params, uint& texRefID){
    stbi_set_flip_vertically_on_load(params.flipTex); //OpenGL reads y/v tex coord in reverse so must flip tex vertically
    glGenTextures(1, &texRefID);
    int currTexRefID;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &currTexRefID);
    glBindTexture(params.texTarget, texRefID); //Make tex referenced by 'texRefIDs[i]' the tex currently bound to the currently active tex unit so subsequent tex commands will config it
    int width, height, colourChannelsAmt;
    unsigned char* data = stbi_load(params.texPath.c_str(), &width, &height, &colourChannelsAmt, 0);
    if(!data){
        return (void)printf("Failed to load tex at \"%s\"\n", params.texPath.c_str());
    }
    glTexImage2D(params.texTarget, 0, colourChannelsAmt == 3 ? GL_RGB16F : GL_RGBA16F, width, height, 0, colourChannelsAmt == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data); //Free the img mem
    glGenerateMipmap(params.texTarget); //Gen required mipmap lvls for currently bound tex
    glTexParameteri(params.texTarget, GL_TEXTURE_WRAP_S, params.texWrapParam);
    glTexParameteri(params.texTarget, GL_TEXTURE_WRAP_T, params.texWrapParam);
    glTexParameteri(params.texTarget, GL_TEXTURE_MIN_FILTER, params.texFilterMin); //Nearest neighbour/Point filtering/interpolation when textures are scaled downwards
    glTexParameteri(params.texTarget, GL_TEXTURE_MAG_FILTER, params.texFilterMag); //Linear filtering/interpolation for upscaled textures
    glBindTexture(params.texTarget, currTexRefID); //Bind previously bound tex
    stbi_set_flip_vertically_on_load(false);
}

void SetUpCubemap(uint& cubemapRefID, const std::vector<cstr>& faces){
    glGenTextures(1, &cubemapRefID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapRefID);
    int width, height, nrChannels;
    unsigned char* data;
    for(GLuint i = 0; i < faces.size(); ++i){ //Gen a tex for each face of the currently bound cubemap
        data = stbi_load(faces[i], &width, &height, &nrChannels, 0);
        if(data){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //All tex targets are looped through this way as the BTS int value of the enums is linearly incremented
        } else{
            printf("Cubemap tex failed to load at path: %s\n", faces[i]);
        }
        stbi_image_free(data);
    }
    ///TexCoords exactly between 2 adj faces of the cubemap might not hit an exact face due to hardware limitations so use GL_CLAMP_TO_EDGE to tell OpenGL to return their edge values when tex sampling between faces
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}