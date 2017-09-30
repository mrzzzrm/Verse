#include <Deliberation/Platform/App.h>

#include "VerseRuntime.h"

using namespace deliberation;

int main(int argc, char * argv[]) {
    InitGlobal<App>()->run<VerseRuntime>("Verse", argc, argv);
    DeleteGlobals();
}
