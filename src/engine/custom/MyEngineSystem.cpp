#include "MyEngineSystem.h"
#include <vector>
using namespace std;

//MyEngineSystem
MyEngineSystem::MyEngineSystem(const GraphicsEngine& gfxInstance) : bufferSize(5), gfx(gfxInstance) { }

LevelFile MyEngineSystem::loadLevel(const string& path, const int& id)
{
    LevelFile* File = new LevelFile(id);

    openRWstream(*path.c_str(), *File);

    return *File;
}
LevelFile MyEngineSystem::loadLevel(const string& path, const int& id, const int& gridsize, const int& width, const int& height)
{
    LevelFile* File = new LevelFile(id, gridsize, width, height);
    gfx.setWindowSize(width, height);

    openRWstream(*path.c_str(), *File);

    return *File;
}

void MyEngineSystem::openRWstream(const char& path, LevelFile& File)
{
    Uint8* buffer = new Uint8[bufferSize];
    int readDivisons = (File.grid.height * File.grid.width) / bufferSize;

    SDL_RWops* FileStream{SDL_RWFromFile(&path, "rb")}; //Open filestream in "read binary" mode

    if (!FileStream) {
        debug("Error Reading Level File: ", SDL_GetError());
        return;
    }

    for (int i = 0; i < readDivisons; i++) {
        SDL_RWread(FileStream, buffer, sizeof(Uint8), bufferSize);
        
        File.parseLevelBinary(buffer, bufferSize, SDL_RWtell(FileStream));
    }

    SDL_RWclose(FileStream);
}

//---------------------------------------------
//LevelFile

//Constructors
LevelFile::LevelFile(const int& id)
{
    // Default values
    metadata.gridsize = 10;
    metadata.pxwidth = 800;
    metadata.pxheight = 600;

    grid.height = metadata.pxheight / metadata.gridsize;
    grid.width = metadata.pxwidth / metadata.gridsize;
}
LevelFile::LevelFile(const int& id, const int& gridsize, const int& width, const int& height)
{
    metadata.id = id;
    metadata.gridsize = gridsize;
    metadata.pxwidth = width;
    metadata.pxheight = height;

    grid.height = height / gridsize;
    grid.width = width / gridsize;
}
//

void LevelFile::parseLevelBinary(Uint8 buffer[], const int& size, const int offset)
{
    std::array<Uint16, 2> coordinates;
    int gridPosition;

    for (int i = 0; i < size; i++)
    {
        gridPosition = (offset - (size - i));

        coordinates[0] = (gridPosition % grid.width) * metadata.gridsize; //X
        coordinates[1] = (gridPosition / grid.width) * metadata.gridsize; //Y

        grid.tiles[buffer[i]].push_back(coordinates);
    }
}
//---------------------------------------------
