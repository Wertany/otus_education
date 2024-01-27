
#include "cmd_config.hpp"
#include "scanner.hpp"

int main(int argc, char *argv[]) {
  homework_8::CMDConfig parser;
  if (!parser.ParseArguments(argc, argv)) {
    return 0;
  }

  homework_8::Scanner scanner(parser);
  scanner.ScanDuplicates();
  scanner.PrintDuplicates();
}
