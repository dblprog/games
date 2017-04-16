--David Lax
--CMSC 161 Lab 9: Mancala-PLaying AI

-- Collaborators: Mikhail Iouchkov, Jacob Burroughs, James Waugh, Jasha Sommer-Simpson, and Zach Krebs.

module MancalaAI(aiNextMove) where

import MancalaBoard
import Data.List  -- for zipping and evaluating booleans
import Data.Maybe -- for fromJust in lookahead and playerNum

type Move = Int

-- Decides what move to make for the current player of the specified MancalaBoard.

heuristicScore :: Player -> MancalaBoard -> Int
heuristicScore player mancala 
	| willWin = 1000
	| otherwise = (2*h1)+h2 + (mult*m)   
							   -- h1 is how many it has in own pit, h2 is difference between pits
	where					  -- accounts for various endgames by solely focusing on getting own 
		h1 = n player         -- pieces in own store, so there are not enough on playerA's side for a playerA win
		h2 = (n player) - (n (np))
		willWin = (numCaptured mancala player) > 24
		n p = numCaptured mancala p
		np = nextPlayer player
		
		m = playerScore mancala player - playerScore mancala np     -- late-game heuristic: how many on each side 
		ops = sum(playerSide mancala np) 
		boths = sum(p player) + sum(playerSide mancala np)              -- sum of pieces left in play
		mult = if (3* ops > 2* boths) then -100 else 0                            -- if few pieces left, close to end-game; weigh heuristic m more!
		p x = playerSide mancala x


aiNextMove :: MancalaBoard -> Move
aiNextMove mancala = lookahead mancala 4      -- greatest depth is optimal... see lookahead for optimization
											  -- (Takes more than 3 seconds if playerA's first move is 5)

evalPosition :: Player -> MancalaBoard -> Int -> Int
evalPosition player mancala 0     = heuristicScore player mancala          -- base case; calculate heuristic at this leaf
evalPosition player mancala depth = (evalPosition player newbrd (depth-1)) -- recurse down by calling lookahead with depth decremented
  where newbrd = move mancala (lookahead mancala (depth-1))

playerNum :: Player -> Int                                                 -- duplicated from MancalaBoard.hs for use in lookahead
playerNum p = fromJust $ elemIndex p allPlayers


lookahead :: MancalaBoard -> Int -> Move
lookahead mancala depth = case allowedMoves mancala of 						-- catch to prevent argmaxes being called on an empty list.
	[] -> dontmove where dontmove = 6																
	_  ->  case b of
		True ->  7*(playerNum player) + (length a -1 - (fromJust $ elemIndex True a)) -- make move closest to pit (to preserve others)
		False -> last $ argmaxes (\l -> evalPosition player (move mancala l) depth)   -- maximize evalPosition function after calling on each move
				                (allowedMoves mancala)
   where player = getCurPlayer mancala
    	 a = reverse $ zipWith (==) p [6,5,4,3,2,1]
    	 b = or a                                   -- if any of a are true, use move closest to store; no need to call evalPosition!
    	 p = playerSide mancala player
