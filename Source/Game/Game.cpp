#include <Deliberation/Platform/Application.h>

#include "VerseApplicationRuntime.h"

using namespace deliberation;

int main(int argc, char * argv[]) {
    Application::get().run(std::make_shared<VerseApplicationRuntime>(), argc, argv);
}
