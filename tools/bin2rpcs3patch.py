import io
import os
import struct
import argparse
import re

def readU8( f: io.FileIO ):
    return struct.unpack( ">B", f.read( 1 ) )[0]

def readU16BE( f: io.FileIO ):
    return struct.unpack( ">H", f.read( 2 ) )[0]

def readU32BE( f: io.FileIO ):
    return struct.unpack( ">I", f.read( 4 ) )[0]

def readU64BE( f: io.FileIO ):
    return struct.unpack( ">Q", f.read( 8 ) )[0]

def readF32BE( f: io.FileIO ):
    return struct.unpack( ">f", f.read( 4 ) )[0]

def readF64BE( f: io.FileIO ):
    return struct.unpack( ">d", f.read( 8 ) )[0]

def hexToInt( s: str ):
    return int( s, 0 )

PATCH_SECTION_START = r'bin2rpcs3patch begin (\w+)$'
PATCH_SECTION_END = 'bin2rpcs3patch end'

def preprocessPatchFile( output, patchName ):
    header = ""
    footer = ""
    state = 'header'
    
    # find patch header/footer
    with open( output ) as f:
        l = f.readline()
        while l != '':
            if state == 'header':
                # append header
                header += l
                
                # look for section start
                res = re.search( PATCH_SECTION_START, l )
                if res:
                    name = res.group( 1 )
                    if name == patchName:
                        state = 'patch' 
            
            if state == 'patch':
                # look for section end
                if PATCH_SECTION_END in l:
                    state = 'footer'
            
            if state == 'footer':
                # append footer
                footer += l

            l = f.readline()
           
    success = state != 'header' 
    return success, header, footer

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', type=str, nargs='+', help="input file path(s)", required=True)
    parser.add_argument('--address', type=hexToInt, nargs='+', help='output virtual address(es), 1 per file (hex 0x)', required=True)
    parser.add_argument('--ppu_hash', type=str, help='ppu hash (ex. PPU-989d32f6687bc20a43cb123a62fc7bc72c3890a8)')
    parser.add_argument('--title', type=str, help='patch title')
    parser.add_argument('--games', type=str, help='comma separated list of games (ex. "Persona 5",BLES02247,NPEB02436)')
    parser.add_argument('--author', type=str, help='author name')
    parser.add_argument('--notes', type=str, help='patch notes')
    parser.add_argument('--version', type=str, help='patch version (ex. 1.0)')
    parser.add_argument('--indent', type=int, help='initial indentation level', default=0)
    parser.add_argument('--output', type=str, help="output file path (default is stdout)")
    parser.add_argument('--append', action='store_true', help='enable to append to the output file instead of overwriting')
    parser.add_argument('--max_value_size', type=int, help='max value size used by patch', default=4)
    parser.add_argument('--float', action='store_true', help='force data to be interpreted as floats (if possible)')
    parser.add_argument('--ascii', action='store_true', help='interpret input as ASCII string')
    parser.add_argument('--replace_patch', type=str, help=f'replaces a patch section in the output file (syntax: #{PATCH_SECTION_START} <name> {PATCH_SECTION_END})')
    parser.add_argument('--add_patch', action='store_true', help='adds patch section if it does not exist')
    args = parser.parse_args()
    
    header = ""
    footer = ""
    
    outputFile = None
    if args.output != None:
        if args.append:
            outputFile = open( args.output, 'a+' )
        else:
            if args.replace_patch != None:
                success, header, footer = preprocessPatchFile( args.output, args.replace_patch )
                if not success:
                    if args.add_patch:
                        # add new section
                        header = f'bin2rpcs3patch begin { args.replace_patch }'
                        footer = f'bin2rpcs3patch end'
                    else:
                        raise Exception( f"Requested path to replace was not found in the output file: {args.replace_patch}" )
                
            outputFile = open( args.output, 'w+' )
            outputFile.write( header )
    
    def write( indent, s ):
        formattedStr = ' ' * ( indent * 2 ) + s
        if outputFile == None:
            print( formattedStr )
        else:
            outputFile.write( formattedStr + '\n' )

    indent = args.indent
    if args.ppu_hash != None:
        write( indent, args.ppu_hash + ':' )
        indent += 1
        
    if args.title != None:
        write( indent, args.title + ':' )
        indent += 1
    
    if args.games != None:
        write( indent, 'Games:' )
        indent += 1
        
        games = args.games.split( ',' )
        write( indent, f'"{games[0]}":')
        indent += 1
        for i in range( 1, len(games) ):
            write( indent, f'{games[i]}: [ All ]' )
        indent -= 1
        
        indent -= 1
        
    if args.author != None:
        write( indent, f'Author: {args.author}')
        
    if args.notes != None:
        write( indent, f'Notes: {args.notes}')
        
    if args.version != None:
        write( indent, f'Patch Version: {args.version}')
        
    if args.ppu_hash != None:
        write( indent, 'Patch:' )
        indent += 1
        
    def writePatchVal( baseAddr, valSize ):
        val = None
        valType = ''      
        valOff = fIn.tell()
              
        if valSize == 1:
            val = fIn.read( 1 )[0]
            valType = 'byte'
        elif valSize == 2:
            val = readU16BE( fIn )
            valType = 'be16'
        elif valSize == 4:
            if args.float:
                val = readF32BE( fIn )
                valType = 'bef32'
            else:
                val = readU32BE( fIn )
                valType = 'be32'
        elif valSize == 8:
            if args.float:
                val = readF64BE( fIn )
                valType = 'bef64'
            else:
                val = readU64BE( fIn )
                valType = 'be64'
        else:
            off = 0
            remValSize = valSize
            while remValSize >= 0:
                if remValSize >= 8:
                    writePatchVal( baseAddr, 8 )
                    remValSize -= 8
                    
                if remValSize >= 4:
                    writePatchVal( baseAddr, 4 )
                    remValSize -= 4
                    
                if remValSize >= 2:
                    writePatchVal( baseAddr, 2 )
                    remValSize -= 2
                    
                if remValSize >= 1:
                    writePatchVal( baseAddr, 1 )
                    remValSize -= 1
            return
        
        write( indent, f'- [ {valType}, {hex(baseAddr + valOff)}, {hex(val)} ]')
        
    for i, inputPath in enumerate( args.input ):
        baseAddr = args.address[min(i, len(args.address) - 1)]
        
        isTempFile = False
        if args.ascii:
            # write temp binary file
            with open( "temp.bin", "wb" ) as fOut:
                fOut.write( inputPath.encode( "ascii" ) )
                fOut.write( b'\x00' )
                inputPath = "temp.bin"
                isTempFile = True
                
        with open( inputPath, "rb" ) as fIn:
            fileSize = os.path.getsize( inputPath )
            while fIn.tell() < fileSize:
                valSize = min( fileSize - fIn.tell(), args.max_value_size )
                writePatchVal( baseAddr, valSize )
                
        if isTempFile:
            os.remove( inputPath )

    if outputFile != None:
        outputFile.write( footer )
        outputFile.close()


if __name__ == '__main__':
    main()