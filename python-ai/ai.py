import socket
import time
import uuid


class TicTacToeAI:
    def __init__(self):
        self.opponent = "X"
        self.player = "O"

        self.scores = {
            self.player: 1,
            self.opponent: -1,
        }

    def checkWinner(self, board: list[str]):
        # fmt: off
        winning_lines = [
            [0, 1, 2], [3, 4, 5], [6, 7, 8],
            [0, 3, 6], [1, 4, 7], [2, 5, 8],
            [0, 4, 8], [2, 4, 6]
        ]
        # fmt: on

        for line in winning_lines:
            a, b, c = line
            if board[a] == board[b] == board[c] != " ":
                return board[a]

        return None

    def minmax(self, board: list[str], depth: int, maxPlayer: bool):
        winner = self.checkWinner(board)
        if winner is not None:
            return self.scores[winner]

        if depth == 0:
            return 0

        if maxPlayer:
            maxEval = -99999
            for i in range(9):
                if board[i] != " ":
                    continue

                board[i] = self.player
                eval = self.minmax(board, depth - 1, False)
                board[i] = " "

                if eval > maxEval:
                    maxEval = eval

            return maxEval

        else:
            minEval = 99999
            for i in range(9):
                if board[i] != " ":
                    continue

                board[i] = self.opponent
                eval = self.minmax(board, depth - 1, True)
                board[i] = " "

                if eval < minEval:
                    minEval = eval

            return minEval

    def generateMove(self, move: str):
        board = [x for x in move.split("\\") if x != ""]

        print(f"Parsed board: {board}")

        depth = 0
        for tile in board:
            if tile == " ":
                depth += 1

        maxEval = -99999
        bestMove = 0

        for i in range(9):
            if board[i] != " ":
                continue

            board[i] = self.player
            eval = self.minmax(board, depth - 1, False)
            board[i] = " "
            print(f"Board:{board},Eval:{eval}")

            if eval > maxEval:
                maxEval = eval
                bestMove = i

        print(bestMove)
        return bestMove


def main():
    print("Python AI ver 1.0")
    ai = TicTacToeAI()

    s = socket.socket()
    port = 8080

    s.connect(("127.0.0.1", port))
    id = uuid.uuid4().hex[:8]

    file_name = f"python-ai/results/match_{id}.txt"
    print(file_name)

    with open(file_name, "w") as file:
        while True:
            text = s.recv(1024).decode()
            print(text)
            result = text.split(":")

            if result[0] == "NAME":
                if result[1] == "X":
                    ai.player = "X"
                    ai.opponent = "O"
                file.write(f"player {result[1]}\n")
                file.write("move,response_time\n")

            if result[0] == "MOVE":
                start_time = time.time()
                move = ai.generateMove(result[1])
                final_time = time.time() - start_time
                file.write(f"{move},{final_time}\n")
                s.send(str(move).encode())

            if result[0] == "Win":
                file.write(f"winner {result[1]}\n")

            if result[0] == "Draw":
                file.write("draw\n")

            if result[0] == "EXIT":
                break

    s.close()


if __name__ == "__main__":
    main()
