import argparse

def write_patch(patch_file, original_elf, output_file):
    elfmem = None
    with open(original_elf, "rb") as elf:
        elfmem = bytearray(elf.read())

    with open(patch_file, "r") as txt:
        with open(output_file, "wb+") as elf:
            while True:
                line = txt.readline()
                if line.strip() == "":
                    break

                if line[0] == "#" or line[0] == "\n" or line[0] == "\r":
                    continue

                where, what = line.split(": ")
                where = int(where, 0) - 0x10000 # base address in memory is 0x10000

                try:
                    what = int(what, 0).to_bytes(4, "big")
                    print(f"Applied {what} to {str(where)}")
                except ValueError:
                    with open(what.strip(), "rb") as patch:
                        what = patch.read()

                elfmem[where : where + len(what)] = what

            elf.write(elfmem)

def main():
    parser = argparse.ArgumentParser(description="Process binary files")
    parser.add_argument('--input', help='Input binary file', required=True)
    parser.add_argument('--patch', help='Patch file to apply to file', required=True)
    parser.add_argument('--output', help='Output file', required=True)

    args = parser.parse_args()

    write_patch(args.patch, args.input, args.output)

if __name__ == "__main__":
    main()