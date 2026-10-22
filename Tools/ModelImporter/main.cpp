#include <GomiEngine/Inc/GomiEngine.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <cstdio>

using namespace GomiEngine;
using namespace GomiEngine::Graphics;
using namespace GomiEngine::Math;

struct Arguments
{
    std::filesystem::path inputFileName;
    std::filesystem::path outputFileName;
    float scale = 1.0f;
};

std::optional<Arguments> ParseArgs(int argc, char* argv[])
{
    if (argc < 1)
    {
        return std::nullopt;
    }

    Arguments args;
    args.inputFileName = argv[argc - 2];
    args.outputFileName = argv[argc - 1];
    for (int i = 0; i + 2 < argc; ++i)
    {
        if (strcmp(argv[i], "-scale") == 0)
        {
            args.scale = atof(argv[i + 1]);
            ++i;
        }
    }
    return args;
}

// <scale> <scalevalue> <inputfilename> <outputfilename>
int main(int argc, char* argv[])
{
    const auto argsOpt = ParseArgs(argc, argv);
    if (!argsOpt.has_value())
    {
        printf("Not enough arguments, import fbs failed.\n");
        return -1;
    }

    printf("Begin Import\n");
    const Arguments& args = argsOpt.value();
    
    //use assimp
    Assimp::Importer importer;
    // this is for animation, does not save pivot deviations
    importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
    const uint32_t flags = aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded;
    const aiScene* scene = importer.ReadFile(args.inputFileName.u8string().c_str(), flags);

    if (scene == nullptr)
    {
        printf("Error: %s\n", importer.GetErrorString());
        return -1;
    }
    printf("Importing %s...\n", args.inputFileName.u8string().c_str());

    return 0;
}