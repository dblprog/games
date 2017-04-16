--David Lxa
--CMSC 161
--Lab 7: Mancala Part 1

--Collaborators: Michael Iouchkov, Zach Krebs, Yasha 

module Main (MancalaBoard, Player, initial, getCurPlayer,
            getBoardData, numCaptured, move, allowedMoves, isAllowedMove,
            gameOver, winners, main) where

import Data.List as List 
-- for List.elemIndex
import Data.Maybe as Maybe 
-- for List.elemIndex

import System.Environment
import Text.Read 
-- for play

{-
 - The stones on a Mancala board are simply recorded as a list of Ints.
   The Ints come in the following order:
 - 1. The boardSize pits belonging to PlayerA [0,1,2,3]
 - 2. The store belonging to PlayerA          [4]
 - 3. The boardSize pits belonging to PlayerB [5,6,7,8]
 - 4. The store belonging to PlayerB          [9]
 -}

data MancalaBoard = MancalaBoardImpl [Int] Player

data Player = PlayerA | PlayerB deriving (Eq, Show)

---- Functions/constants for Player ----

allPlayers = [PlayerA, PlayerB]
numPlayers = length allPlayers

playerNum :: Player -> Int
playerNum p = fromJust $ List.elemIndex p allPlayers

playerWithNum :: Int -> Player
playerWithNum i = allPlayers !! i

-- Find the player whose turn is next 
nextPlayer :: Player -> Player
nextPlayer p = playerWithNum $ ((playerNum p) + 1) `mod` numPlayers

---- Functions/constants for MancalaBoard ----

boardSize = 6 -- number of pits on each side
startStones = 4 -- number of stones in each pit

{- the initial mancala board -}
initial :: MancalaBoard
initial = MancalaBoardImpl (concat $ take numPlayers (repeat boardSide)) PlayerA
                        -- One side of board                pit at end
    where boardSide = take boardSize (repeat startStones) ++ [0]


{- return the index of the first pit belonging to a player -}
indexForFirstPit :: Player -> Int
indexForFirstPit p = (playerNum p) * (boardSize + 1)


{- return the index of the store for that player -}
indexForPlayerStore :: Player -> Int
indexForPlayerStore p = boardSize + (indexForFirstPit p)


{- return the indices for the pits (without the store) for a player -}
indicesForPlayerSide :: Player -> [Int]
indicesForPlayerSide p = [firstPit .. lastPit] where
    firstPit = indexForFirstPit p
    lastPit = firstPit + boardSize - 1


--Functions written as per to-do prompts:
--Note: ALL TESTS FOR IMPLEMENTED FUNCTIONS AT END OF FILE, 
-- INCLUDING A testAll function

---- Retrieve information about Mancala Board

-- return the player who has the current turn 
getCurPlayer :: MancalaBoard -> Player
getCurPlayer (MancalaBoardImpl pits plyr) = plyr


-- return the list of all pits in the board -
getBoardData :: MancalaBoard -> [Int]
getBoardData (MancalaBoardImpl pits _) = pits


--return the side of the board for a specified player, including the player's store
playerSide :: MancalaBoard -> Player -> [Int]
playerSide (MancalaBoardImpl pits _) p1 = 
	if (p1 == PlayerA)
	then take 7 pits 
	else drop 7 pits

--return the number of captured pieces in specified player's store 
numCaptured :: MancalaBoard -> Player -> Int 
numCaptured a py = last $ playerSide a py 

--returns a list of valid moves for the current player:
-- ie. the indices of pits which belong to that player, and which contain one or more pieces

allowedMoves :: MancalaBoard -> Player -> [Int]
allowedMoves board player = map (+(7* (playerNum player)) ) (filter ((>0) . ((playerSide board player) !!)) [0..5] )

--check that a move is valid for the current player 
isAllowedMove :: MancalaBoard -> Int -> Bool
isAllowedMove m@(MancalaBoardImpl board player) mv = elem mv $ allowedMoves m player

-- We number the pits from 0 to 13 (2 players, 6 pits each and 1 store each)
-- This function takes a board and applies the move where the player selects
-- the numbered pit, giving back an updated board after the move

move :: MancalaBoard -> Int -> MancalaBoard
move (MancalaBoardImpl b1 py) m1 = spreadStones (MancalaBoardImpl b2 (py)) (m1+1) num
	where  (b2, num) = emptyWell b1 m1

spreadStones :: MancalaBoard -> Int -> Int -> MancalaBoard
spreadStones m@(MancalaBoardImpl b1 py) ind nstones 
  | nstones == 0 = MancalaBoardImpl b1 (nextPlayer py)
  | nstones == 1 && (oppStore (nextPlayer py) ind) = mbIncr
  | otherwise = if (oppStore py ind) 
                then spreadStones m indInc (nstones)
  	            else spreadStones (mbIncr) indInc (nstones-1)

    where mbIncr = MancalaBoardImpl (incrementWell b1 ind) py 
          indInc = (ind+1) `mod` 14

oppStore :: Player -> Int -> Bool
oppStore py ind                 -- note: opposite implementation because of change in move
  | (py == PlayerA) && (ind == 13) = True
  | (py == PlayerB) && (ind == 6)  = True
  | otherwise                      = False 

emptyWell :: [Int] -> Int -> ([Int], Int)
emptyWell b1 ind = (((take ind b1)++[0]++(drop (ind+1) b1)), (b1 !! ind))

incrementWell :: [Int] -> Int -> [Int]
incrementWell b1 ind = ((take ind b1)++[((b1 !! ind)+1)]++(drop ((ind+1)) b1))

--gameOver checks to see if the game is over (i.e. if one player's side of the board is all empty 

gameOver :: MancalaBoard -> Bool
gameOver m@(MancalaBoardImpl board player) = ((allowedMoves m player) == []) 

--winners returns a list of players who have the top score: there will only be 
-- one in the list if there is a clear winner, and none if it is a draw
  
winners :: MancalaBoard -> [Player]
winners a 
  | numA < numB = [PlayerB]
  | numA > numB = [PlayerA]
  | otherwise   = []
	where 
		numA = numCaptured a PlayerA 
		numB = numCaptured a PlayerB 

move1_1 :: MancalaBoard
-- move1_1, move2_2, etc: 2 different sets of moves 
-- in games 1 and 2 for various tests (below)

move1_1 = move initial 1
move1_8 = move move1_1 8
move1_2 = move move1_8 2

move2_2 = move initial 2
move2_3 = move move2_2 3
move2_9 = move move2_3 9
move2_8 = move move2_9 8
move2_12 = move move2_8 12

--endTie, etc: a series of end-games for testing

endTie  = MancalaBoardImpl [0,0,0,0,0,0,24,0,0,0,0,0,0,24] PlayerA
endWinA = MancalaBoardImpl [0,0,2,1,0,0,25,3,2,0,0,0,0,15] PlayerB
endWinB = MancalaBoardImpl [0,0,0,0,0,0,8,2,1,0,3,1,0,33]  PlayerB

---- show, which now pretty prints the board and game state

instance Show MancalaBoard where
    show (MancalaBoardImpl boardData player) = -- (show boardData) ++ " " ++ (show player)

             open  ++ blank ++ pBI ++ line0 ++line1 ++ line2 ++
             line3 ++ line4 ++ blank ++ pAI ++ close ++ tabs
                   ++ "It's " ++show player ++"'s turn! \n"

      where 
        b = boardData
        r = replicate 
        
        open  = "\n " ++ r 75 '_' ++ "\n"
        blank = "/" ++ r 74 ' ' ++ lEnd
        line0 = "/  " ++ r 8 '_' ++  cr6 ++ r 10 ' ' ++ lEnd
        line1 = short2 line1a 12 line1b (reverse [7..11]) ++ r 8 '_'++ lEnd
        line2 = "/  |  " ++ fill 13 ++ show (b !! 13) ++ spc ++ "    |" ++ lEnd
        line3 = "/  |______|" ++ cr6 ++ "  |  " ++ fill 6 ++ show (b !! 6) ++ "  |" ++ lEnd
        line4 = short2 line4a 0 line1b [1..5] ++ "|______|" ++ lEnd
        close = "/" ++ r 75 '_' ++ "\\ \n \n" 
        
        pBI = "/    PB  " ++ pHelp 7 ["12","11","10","09","08","07"] ++ r 12 ' ' ++ lEnd
        pAI = "/        " ++ pHelp 7 ["00","01","02","03","04","05"] ++ "        PA  " ++ lEnd

        line1a = "/  |      |   [_"
        line1b = "_]   [_"
        line4a = "/" ++ r 13 ' ' ++ "[_"
        
        spc = "  |" ++ r 56 ' ' ++ "|  "
        cr6 = concat (r 6 (r 3 ' ' ++ r 6 '_'))
        tabs = "\t \t \t"
        lEnd = " \\ \n"

        pHelp num (i:is) = case length(i:is) of
          1 -> y 
          _ -> y ++ pHelp num (is)
          where y = r num ' ' ++ i

        short str (i:is) = case length(i:is) of
            1 -> x 
            _ -> x ++ short str is
          where x = str ++ fill i ++ show (b !! i)

        short2 str1 ind1 str2 lst = short str1 [ind1] ++ short str2 lst ++ "_]  "
        
        fill i 
          | ((b!!i) < 10) = "0"
          | otherwise = ""


-- Code to run the game: a run (main) function, a loop, and 
-- a function to congratulate the winner!


congWinner :: MancalaBoard -> String 
congWinner m = case winners m of
    []        -> "A draw! Well matched!"
    [PlayerA] ->  cString ++ "PlayerA!!!" 
    [PlayerB] ->  cString ++ "PlayerB!!!"
  where cString = "We have a winner! Congratulations to " 

loopGame :: MancalaBoard -> IO ()
loopGame m@(MancalaBoardImpl b py) = do
    putStrLn $ "\t \t \t Board is now:" ++ show m
    
    if(gameOver m) then putStrLn (congWinner m)
    else do 
      putStrLn ((show $ getCurPlayer m) ++ ", where do you want to move?")   
      movM <- getLine >>= (return . readMaybe )
      
      if (isNothing movM) then tryAgain
      else do 
        let mv = fromJust movM 
        
        if (isAllowedMove m mv) then loopGame (move m mv)
        else tryAgain
        where 
          tryAgain = do putStrLn("Invalid move--try again!")
                        loopGame m 

main :: IO ()
main = do 
    putStrLn "Welcome to Mancala! Here is the initial board:"
    let b0 = initial
    loopGame b0
    putStrLn "Good game! To play again, just type './playMancala!' \n"
                                                                                        

-- Instantiate board to play game.
-- NOTE: For testing, call loopGame on bi, the game you'd like to 'play' with
--    let b1 = MancalaBoardImpl [2,0,1,0,2,1,16,0,3,4,0,2,2,15] PlayerA
--    let b2 = MancalaBoardImpl [1,1,1,1,3,1,19,0,0,0,1,0,0,20] PlayerA
--    let b3 = MancalaBoardImpl [0,0,0,0,0,1,27,2,3,4,5,0,0,6] PlayerA 

{-   Diagram of PrettyPrint
 ____________________________________________________________________     open 
/                                                                    \    blank
/   PB       12      11      10      09      08      07              \    pBI
/ _______   ____    ____    ____    ____    ____    ____             \    line0
/ |     |  [____]  [____]  [____]  [____]  [____]  [____]    _______ \    line1
/ |     |                                                    |     | \    line2
/ |_____|   ____    ____    ____    ____    ____    ____     |     | \    line3
/          [____]  [____]  [____]  [____]  [____]  [____]    |_____| \    line4
/                                                                    \    blank
/            00      01     02       03      04      05         PA   \    pAI
/____________________________________________________________________\    close

-}