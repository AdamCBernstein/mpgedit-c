#!/usr/bin/perl
use mpgtests;

#
# This test script chops up an input mp3 file into individual frames, and
# stores them in random order in a file called "scramble.mp3".  This
# scrambled file is not playable, as all of the frames are stored in 
# a garbled order.  However, the file can be unscrambled using the
# test script 'unscramble.pl'.  This script takes the scramble.mp3 and
# reorders the frames into the correct sequence, using the information
# saved in scramble.out.  scramble.out is a file generated by scramble.pl,
# which associates the frame number with the time offset for that frame.
# scramble.pl and unscramble.pl constitute reciprocal operations.
# This this sequence of commands will result in a descrambled output file
# that is identical to the original input file:
#
#   ./scramble.pl test1.mp3       -> Generates scramble file scramble.mp3
#   ./descramble.pl scramble.mp3  -> Generates descramble.mp3 from scramble.mp3
#   cmp descramble.mp3 test1.mp3  -> Prove descramble.mp3 is same as testl.mp3
#
# These two scripts constitute a real torture test for mpgedit.

# Notes:
# The external program 'scramble' generates the scramble translation
# file, called scramble.out.  scramble currently is implemented in C.
# This should be rewritten in Perl at some point.  The current C
# implementation uses popen() which is not portabile to Win32 platforms.
#
# The last frame of the input file must be specially treated.  This is
# because the actual frame length is usually shorter than the frame header
# indicates.  Should this last frame end up somewhere in the middle of the
# scramble file, this will completely break the decoding of the file.
# This is because this frame is actually shorter than the frame header
# indicates, so there is no way to tell where the actual frame ends.
# These test programs solve this problem by storing the last frame in a
# special end file, scrambles the remaining frames, then pastes the end
# frame onto the end of the scramble file.  The descramble script
# handles the final frame in a similar manner.


$mpgedit = "./mpgedit";

scramble_file_cleanup();

if (length($ARGV[0]) == 0) {
  print("usage: $0 mp3_file\n");
  exit(1);
}
scramble_main($ARGV[0], 1);
