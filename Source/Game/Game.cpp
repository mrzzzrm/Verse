#include <Deliberation/Platform/App.h>

#include "VerseRuntime.h"

using namespace deliberation;

int main(int argc, char * argv[]) {
    App::get().run(std::make_shared<VerseRuntime>(), argc, argv);
}
