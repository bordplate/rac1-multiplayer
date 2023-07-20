import argparse
import struct

def read_binary_file(filename, start_address, output_file, append_mode):
    with open(filename, 'rb') as file:
        address = start_address

        while True:
            bytes = file.read(4)

            if len(bytes) < 4:
                break

            value = struct.unpack('I', bytes[::-1])[0]
            output_file.write(f"0x{address:06x}: 0x{value:08x}\n")

            address += 4

        return


def main():
    parser = argparse.ArgumentParser(description="Process binary files")
    parser.add_argument('--input', nargs='+', help='Input binary files', required=True)
    parser.add_argument('--address', nargs='+', help='Start addresses for each file in hexadecimal format', required=True)
    parser.add_argument('--output', help='Output file', required=True)
    parser.add_argument('--append', action='store_true', help='If set, output will be appended to existing file')

    args = parser.parse_args()

    if len(args.input) != len(args.address):
        raise ValueError("Number of input files and addresses do not match")

    mode = 'a' if args.append else 'w'

    with open(args.output, mode) as output_file:
        for filename, start_address in zip(args.input, args.address):
            read_binary_file(filename, int(start_address, 16), output_file, mode)

if __name__ == "__main__":
    main()