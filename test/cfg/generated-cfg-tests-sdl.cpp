// auto generated tests from cfg/sdl.cfg
//
// Generated by command:
// ./generate_cfg_tests cfg/sdl.cfg > generated-cfg-tests-sdl.cpp
//
// Recommended cppcheck command line:
// $ cppcheck --enable=warning,information --inline-suppr --platform=unix64 generated-cfg-tests-sdl.cpp
// => 'unmatched suppression' warnings are false negatives.
//

void test__SDL_mutexP__noreturn()
{
    int x = 1;
    if (cond) {
        x=100;
        SDL_mutexP(arg1);
    }
    // cppcheck-suppress shiftTooManyBits
    x = 1 << x;
}

void test__SDL_mutexP__arg1__notbool()
{
    // cppcheck-suppress invalidFunctionArgBool
    SDL_mutexP(!x);
}

void test__SDL_mutexP__arg1__notnull()
{
    // cppcheck-suppress nullPointer
    SDL_mutexP(NULL);
}

void test__SDL_mutexV__noreturn()
{
    int x = 1;
    if (cond) {
        x=100;
        SDL_mutexV(arg1);
    }
    // cppcheck-suppress shiftTooManyBits
    x = 1 << x;
}

void test__SDL_mutexV__arg1__notbool()
{
    // cppcheck-suppress invalidFunctionArgBool
    SDL_mutexV(!x);
}

void test__SDL_mutexV__arg1__notnull()
{
    // cppcheck-suppress nullPointer
    SDL_mutexV(NULL);
}

void test__SDL_RWsize__noreturn()
{
    int x = 1;
    if (cond) {
        x=100;
        SDL_RWsize(arg1);
    }
    // cppcheck-suppress shiftTooManyBits
    x = 1 << x;
}

void test__SDL_RWsize__arg1__notbool()
{
    // cppcheck-suppress invalidFunctionArgBool
    SDL_RWsize(!x);
}

void test__SDL_RWsize__arg1__notnull()
{
    // cppcheck-suppress nullPointer
    SDL_RWsize(NULL);
}

void test__SDL_RWread__noreturn()
{
    int x = 1;
    if (cond) {
        x=100;
        SDL_RWread(arg1, arg2);
    }
    // cppcheck-suppress shiftTooManyBits
    x = 1 << x;
}

void test__SDL_RWread__arg1__notbool()
{
    // cppcheck-suppress invalidFunctionArgBool
    SDL_RWread(!x, arg2);
}

void test__SDL_RWread__arg1__notnull()
{
    // cppcheck-suppress nullPointer
    SDL_RWread(NULL, arg2);
}

void test__SDL_RWread__arg2__notbool()
{
    // cppcheck-suppress invalidFunctionArgBool
    SDL_RWread(arg1, !x);
}

void test__SDL_RWread__arg2__notnull()
{
    // cppcheck-suppress nullPointer
    SDL_RWread(arg1, NULL);
}

void test__IMG_isPNG__noreturn()
{
    int x = 1;
    if (cond) {
        x=100;
        IMG_isPNG(arg1);
    }
    // cppcheck-suppress shiftTooManyBits
    x = 1 << x;
}

void test__IMG_isPNG__arg1__notbool()
{
    // cppcheck-suppress invalidFunctionArgBool
    IMG_isPNG(!x);
}

void test__IMG_isPNG__arg1__notnull()
{
    // cppcheck-suppress nullPointer
    IMG_isPNG(NULL);
}

void test__IMG_isJPG__noreturn()
{
    int x = 1;
    if (cond) {
        x=100;
        IMG_isJPG(arg1);
    }
    // cppcheck-suppress shiftTooManyBits
    x = 1 << x;
}

void test__IMG_isJPG__arg1__notbool()
{
    // cppcheck-suppress invalidFunctionArgBool
    IMG_isJPG(!x);
}

void test__IMG_isJPG__arg1__notnull()
{
    // cppcheck-suppress nullPointer
    IMG_isJPG(NULL);
}

