# Steganographic Encoder/Decoder
A program which can encode any file into a .bmp image using a key, and can then be later decoded using the same key 

Based on my First Year Project at Imperial College London as part of the C Module.  

## What is Steganography ?
Steganography  is  the  practice  of  hiding  sensitive  data  in  plain  sight.   For  our  extension  we  set  out  to  developing a tool such that we could conceal any type of file within a .BMP image,  whilst ensuring the new altered image is essentially indistinguishable from the original.  To challenge ourselves, we took the approach of only producing legacy code (without the use of libraries).

## Implementation
To enhance the security of the encryption process, we implemented a password encryption process.This  password  can  be  optionally  specified  using  the  flag '-p' <password>,  and  this  password  is  converted  into  the starting key for the encryption process using a hashing algorithm.
  
Our program is able to encode files of literally any data type.  An example is in './test/out/shakespeare.bmp'.This image has the entire works of Shakespeare (in the form of a .txt file) encoded within itself using the password 'romeo'.  To prove this, run in terminal './extension -d -i test/out/shakespeare.bmp -p romeoa' and you will find a file 'full_shakespeare.txt' has appeared.

For a more detailed description of the encryption process, design implementations and testing, see the 'Extension' section of the 'Report.pdf' file.

## How to use
Can be run using the following terminal commands (Note :  password is optional):  
Encoding:./extension -e -i <*.bmp> <any file> -o <*.bmp> -p <password>  
Decoding:./extension -d -i <*.bmp> -p <password>
