David Lax
CMSC 161 Aut 14
---------------------------------------
Lab 8 File: playMancala.hs

==============================================

Implements mancala game from last week, with an improved
loop to play the game from start until finish (when one player wins,
or when there is a draw). 

To run, simply compile via

   	$ ghc playMancala.hs

and then run with

         $ ./playMancala

If given invalid input, rather than crashing,
the game will prompt the player to input a move again.

The moves (indices) are shown on the board, with every pit 
on one side of each player's store (labeled)

Game loop also prints informative messages 
(whose turn it is and congratulations the winner(s).


Note: Game loop terminates when one player wins in
the 'winners' function of the file--whichi is only
called when either one player can no longer move 
(NOT when one player gets more than 25 stones in 
his/her store, though this does guarantee a win).


=======================================================
	
Collaborators: Mikhail Iouchkov, Jasha Sommer-Simpson, 
               and Jacob Burroughs
	                      
=======================================================

