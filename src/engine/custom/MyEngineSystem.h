#ifndef __MY_ENGINE_H__
#define __MY_ENGINE_H__

#include <SDL_mixer.h>
#include <array>
#include <vector>

#include "../EngineCommon.h"
#include "../GraphicsEngine.h"

class MyEngineSystem;
class Scene;
class LevelFile;

class MyEngineSystem {
    friend class XCube2Engine;
    private:
        const int bufferSize;

        /**
        * Opens a read/write filestream using SDL_RWFromFile to read the level binary,
        * @param path : the path to a level .bin file containing the level structures,
        * @param File : The LevelFile object that will store the data from the level binary once it has been parsed,
        */
        void openRWstream(const char& path, LevelFile& File);

        /**
        * Requires graphics engine instance for controlling the window size,
        * @param gfxInstance : the graphics engine instance,
        */
        MyEngineSystem(const GraphicsEngine& gfxInstance);
    protected:
        GraphicsEngine gfx;
    public:
        /**
        * Functions that couldn't be implemented within timescale due to complexity.
        */
        //void LoadAudio();
        //void LoadTileset();

        /**
        * \Deserialises the level file and returns a grid struct,
        * @param binPath : the path to a level .bin file containing the level structures,
        * @param id : a levels unique identifier,
        */
        LevelFile loadLevel(const std::string& path, const int& id);
        /**
        * \Deserialises the level file and returns a grid struct,
        * @param binPath : the path to a level .bin file containing the level structures,
        * @param id : a levels unique identifier,
        * @param gridsize : the amount of pixels^2 each tile will take up,
        * e.g. 10 will mean each tile is 10px by 10px,
        * @param width : the horizontal size of the level in tiles,
        * @param height : the vertical size of the level in tiles,
        */
        LevelFile loadLevel(const std::string& path, const int& id, const int& gridsize, const int& width, const int& height);
};

class LevelFile {
    friend class MyEngineSystem;
    protected:
        /**
        * Structure that contains level metadata,
        * id : a levels unique identifier,
        * gridsize : the amount of pixels^2 each tile will take up,
        * pxwidth : pixel width of the level,
        * pxheight : pixel height of the level,
        */
        struct META{
            int id, gridsize, pxwidth, pxheight;
        } metadata;
        
        /**
        * Structure that represents the level grid that is loaded into memory,
        * tiles[] : An array of 255 vectors that hold 2 integer values,
        * Each vector in the array represents a type of tile, and the 2 integers represent it's location in 2 dimensions,
        */
        struct GRID{
            std::vector<std::array<Uint16, 2>> tiles[255];
            int width, height;
        } grid;

        /**
        * Takes the passed section of level binary and adds it to the grid structure with the correct index and coordinates,
        * @param buffer[] : the filestream buffer from reading the level binary,
        * @param size : the size of the buffer,
        * @param offset : the amount of binary that's already been read,
        */
        void parseLevelBinary(Uint8 buffer[], const int& size, int offset);

        /**
        * An object that represents a 2 dimensional game level loaded in memory.
        * @param id : a levels unique identifier,
        */
        LevelFile(const int& id);
        /**
        * An object that represents a 2 dimensional game level loaded in memory.
        * @param id : a levels unique identifier,
        * @param gridsize : the amount of pixels^2 each tile will take up,
        * e.g. 10 will mean each tile is 10px by 10px,
        * @param width : the horizontal size of the level in tiles,
        * @param height : the vertical size of the level in tiles,
        */
        LevelFile(const int& id, const int& gridsize, const int& width, const int& height);
    public:
        META getMetadata() { return metadata; }
        GRID getGrid() { return grid; }
        int getID() { return metadata.id; }
        int getGridsize() { return metadata.gridsize; }
        int getWidth() { return metadata.pxwidth; }
        int getHeight() { return metadata.pxheight; }
};
#endif
