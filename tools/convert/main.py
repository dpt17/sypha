import argparse

from syphapy.format import Formatter, FormatType

if __name__ == "__main__":
    argParser = argparse.ArgumentParser()
    argParser.add_argument("-i",  "--input", type=str, required=True)
    argParser.add_argument("-o", "--output", type=str, required=True)
    args = argParser.parse_args()

    formatter : Formatter = Formatter.load_file(args.input)
    formatter.dump_file(args.output)
