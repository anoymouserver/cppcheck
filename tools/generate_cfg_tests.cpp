
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include "tinyxml2.h"

static void testfunction(const tinyxml2::XMLElement *node, const std::string &functionName);

static std::vector<std::string> splitString(const char *str, char delim)
{
    std::vector<std::string> ret;
    while (const char *p = std::strchr(str,delim)) {
        ret.push_back(std::string(str, p-str));
        str = p + 1;
    }
    ret.push_back(str);
    return ret;
}

static std::string replaceCharInString(std::string s, char from, char to)
{
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == from)
            s[i] = to;
    }
    return s;
}

int main(int argc, char **argv)
{
    if (argc != 2 || !std::strstr(argv[1], ".cfg")) {
        std::cerr << "no cfg file\n";
        return 1;
    }

    const char *cfgfile = argv[1];

    tinyxml2::XMLDocument doc;
    if (tinyxml2::XML_SUCCESS != doc.LoadFile(cfgfile)) {
        std::cerr << "failed to load cfg file\n";
        return 1;
    }

    std::string testfile(cfgfile);
    if (testfile.find('/') != std::string::npos)
        testfile = testfile.substr(testfile.rfind('/')+1);
    testfile = "generated-cfg-tests-" + testfile.substr(0,testfile.find('.')) + ".cpp";

    std::cout << "// auto generated tests from " << cfgfile << std::endl;
    std::cout << "//" << std::endl;
    std::cout << "// Generated by command:" << std::endl;
    std::cout << "// " << argv[0] << ' ' << cfgfile << " > " << testfile << std::endl;
    std::cout << "//" << std::endl;
    std::cout << "// Recommended cppcheck command line:" << std::endl;
    std::cout << "// $ cppcheck --enable=warning,information --inline-suppr --platform=unix64 " << testfile << std::endl;
    std::cout << "// => 'unmatched suppression' warnings are false negatives." << std::endl;
    std::cout << "//" << std::endl << std::endl ;

    const tinyxml2::XMLElement * const rootnode = doc.FirstChildElement();

    for (const tinyxml2::XMLElement *node = rootnode->FirstChildElement(); node; node = node->NextSiblingElement()) {
        const std::string nodename = node->Name();
        if (nodename == "memory" || nodename == "resource") {
            //testalloc(node);
        } else if (nodename == "function") {
            for (const std::string &name : splitString(node->Attribute("name"), ','))
                testfunction(node, name);
        }
    }

    return 0;
}

static std::string functionCall(const std::string &functionName, bool useretval, int argNr, int numberOfArgs, const char badArg[])
{
    std::ostringstream ostr;
    if (useretval)
        ostr << "result = ";
    ostr << functionName << '(';
    for (int i = 1; i <= numberOfArgs; ++i) {
        if (i > 1)
            ostr << ", ";
        if (i == argNr)
            ostr << badArg;
        else
            ostr << "arg" << i;
    }
    ostr << ')';
    return ostr.str();
}

static std::string testFunctionArg(const std::string &functionName, bool useretval, const char type[], int argNr, int numberOfArgs, const char code[], const char suppress[], const char badArg[])
{
    std::ostringstream ostr;
    ostr << "void test__" << replaceCharInString(functionName,':','_') << "__arg" << argNr << "__" << type << "() {" << std::endl;
    if (*code)
        ostr << "  " << code << std::endl;
    ostr << "  // cppcheck-suppress " << suppress << std::endl;
    ostr << "  " << functionCall(functionName, useretval, argNr, numberOfArgs, badArg) << ';' << std::endl;
    ostr << '}';
    return ostr.str();
}

static void testfunction(const tinyxml2::XMLElement *node, const std::string &functionName)
{
    // How many args does this function take?
    int numberOfArgs = 0;
    for (const tinyxml2::XMLElement *child = node->FirstChildElement(); child; child = child->NextSiblingElement()) {
        if (std::strcmp(child->Name(), "arg") == 0) {
            const char *nr = child->Attribute("nr");
            if (nr && *nr >= (numberOfArgs + '0') && *nr <= '9')
                numberOfArgs = *nr - '0';
        }
    }

    bool noreturn = false;
    bool useretval = false;
    bool pure = false;
    //bool constant = false;
    bool leakignore = false;
    for (const tinyxml2::XMLElement *child = node->FirstChildElement(); child; child = child->NextSiblingElement()) {
        if (std::strcmp(child->Name(), "noreturn") == 0)
            noreturn = true;
        else if (std::strcmp(child->Name(), "use-retval") == 0)
            useretval = true;
        else if (std::strcmp(child->Name(), "pure") == 0)
            pure = true;
        //else if (std::strcmp(child->Name(), "const") == 0)
        //    constant = true;
        else if (std::strcmp(child->Name(), "leak-ignore") == 0)
            leakignore = true;
    }

    if (noreturn) {
        std::cout << "void test__" << replaceCharInString(functionName,':','_') << "__noreturn() {" << std::endl;
        std::cout << "  int x = 1;" << std::endl;
        std::cout << "  if (cond) { x=100; " << functionCall(functionName, useretval, -1, numberOfArgs, "") << "; }" << std::endl;
        std::cout << "  // cppcheck-suppress shiftTooManyBits" << std::endl;
        std::cout << "  x = 1 << x;" << std::endl;
        std::cout << "}" << std::endl << std::endl;
    }
    if (useretval) {
        std::cout << "void test__" << replaceCharInString(functionName,':','_') << "__useretval() {" << std::endl;
        std::cout << "  // cppcheck-suppress ignoredReturnValue" << std::endl;
        std::cout << "  " << functionCall(functionName, false, -1, numberOfArgs, "") << ';' << std::endl;
        std::cout << "}" << std::endl << std::endl;
    }
    if (pure) {
        std::cout << "void test__" << replaceCharInString(functionName,':','_') << "__pure(";
        for (int i = 1; i <= numberOfArgs; ++i)
            std::cout << (i>1?",":"") << "int arg" << i;
        std::cout << ") {" << std::endl;
        std::cout << "  // cppcheck-suppress incorrectLogicOperator" << std::endl;
        std::cout << "  if ((" << functionCall(functionName, false, -1, numberOfArgs, "") << " > 10) || (";
        std::cout << functionCall(functionName, false, -1, numberOfArgs, "") <<  " < 100)) {}" << std::endl;
        std::cout << "}" << std::endl << std::endl;
    }
    if (leakignore && functionName.find("::") == std::string::npos) {
        std::cout << "void test__" << replaceCharInString(functionName,':','_') << "__leakignore() {" << std::endl;
        std::cout << "  char *p = malloc(10); *p=0;" << std::endl;
        std::cout << "  " << functionCall(functionName, useretval, 1, numberOfArgs, "p") << ';' << std::endl;
        std::cout << "  // cppcheck-suppress memleak" << std::endl;
        std::cout << "}" << std::endl << std::endl;
    }

    // Generate tests for args
    for (int argNr = 1; argNr <= numberOfArgs; ++argNr) {
        for (const tinyxml2::XMLElement *child = node->FirstChildElement(); child; child = child->NextSiblingElement()) {
            if (std::strcmp(child->Name(), "arg") != 0)
                continue;
            const char *nrAttr = child->Attribute("nr");
            if (!nrAttr || *nrAttr != (argNr + '0'))
                continue;

            bool notbool = false;
            bool notuninit = false;
            bool notnull = false;

            for (const tinyxml2::XMLElement *argCheck = child->FirstChildElement(); argCheck; argCheck = argCheck->NextSiblingElement()) {
                if (std::strcmp(argCheck->Name(), "not-bool") == 0)
                    notbool = true;
                else if (std::strcmp(argCheck->Name(), "not-null") == 0)
                    notnull = true;
                else if (std::strcmp(argCheck->Name(), "not-uninit") == 0)
                    notuninit = true;
            }

            if (notbool) {
                std::cout << testFunctionArg(functionName, useretval, "notbool", argNr, numberOfArgs, "", "invalidFunctionArgBool", "!x") << std::endl << std::endl;
            }

            if (notnull) {
                std::cout << testFunctionArg(functionName, useretval, "notnull", argNr, numberOfArgs, "", "nullPointer", "NULL") << std::endl << std::endl;
            }

            if (notuninit) {
                const char *code = notnull ? "int x[10];" : "int x;";
                std::cout << testFunctionArg(functionName, useretval, "notuninit", argNr, numberOfArgs, code, "uninitvar", "x") << std::endl << std::endl;
            }
        }
    }
}
