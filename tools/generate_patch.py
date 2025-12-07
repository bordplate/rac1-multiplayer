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
    parser.add_argument('--input', nargs='+', help='Input binary files')
    parser.add_argument('--input_file', help='Newline-divided file with input binary files')
    parser.add_argument('--address', nargs='+', help='Start addresses for each file in hexadecimal format')
    parser.add_argument('--address_file', help='Newline-divided file with start addresses for each file in hexadecimal format')
    parser.add_argument('--output', help='Output file', required=True)
    parser.add_argument('--append', action='store_true', help='If set, output will be appended to existing file')

    args = parser.parse_args()

    inputs = args.input
    addresses = args.address

    if args.input_file and args.address_file:
        with open(args.input_file) as inputs_file, open(args.address_file) as addresses_file:
            for line in inputs_file:
                if line.strip():
                    inputs += [line.strip()]

            for line in addresses_file:
                if line.strip():
                    addresses += [line.strip()]

    if len(inputs) <= 0 or len(addresses) <= 0:
        print(f"Number of inputs are not sufficient.")
        exit(-1)

    if len(inputs) != len(addresses):
        raise ValueError(f"Number of input files and addresses do not match ({len(inputs)} != {len(addresses)})")

    mode = 'a' if args.append else 'w'

    with open(args.output, mode) as output_file:
        for filename, start_address in zip(inputs, addresses):
            read_binary_file(filename, int(start_address, 16), output_file, mode)

if __name__ == "__main__":
    main()