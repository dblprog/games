David Lax
CMSC 161 Aut 14
------------------------------------------------------
Lab 8 Files: Mancala.hs, MancalaBoard.hs, MancalaAI.hs
======================================================

Implements mancala game, with an AI to play as PlayerB.

AI has multiple heuristics for optimal play 
(can force draw against a perfect opponent, wins otherwise)


AI uses a lookahead with depth 4 
(because of hard-coded moves, can always run in < 3 secs.)


Details regarding the heuristics and hard-coded moves 
appear as comments within the file. 

AI is able to force a tie or a win in most every playstyle 
against it (though can still lose if opponent stacks stones
in pit 0, which may be guaranteed given that the ai is playerB)


To run, simply compile via
$ ghc -c MancalaBoard.hs 
$ ghc -c MancalaAI.hs 
$ ghc -o mancala Mancala.hs MancalaBoard.hs MancalaAI.hs 

and then run with 
$ ./mancala 


NOTE: If given invalid input, program will crash (for non-ints)
or make the move (even if that spot is empty)

This functionality was built in to distributed Mancala.hs code.

=======================================================
Collaborators: Mikhail Iouchkov, Jasha Sommer-Simpson, 
Zach Krebs, James Waugh, and Jacob Burroughs
=======================================================