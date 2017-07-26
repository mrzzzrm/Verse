#include <iostream>

#include "VerseApplication.h"

using namespace deliberation;

class Game : public VerseApplication
{
public:
    Game() : VerseApplication("Verse") {}
};

int main(int argc, char * argv[]) { return Game().run(argc, argv); }
