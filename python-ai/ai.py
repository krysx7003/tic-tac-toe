opponent = "X"
player = "O"

scores = {
    player: 1,
    opponent: -1,
}


def checkWinner(board: list[str]):
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


def minmax(board: list[str], depth: int, maxPlayer: bool):
    winner = checkWinner(board)
    if winner is not None:
        return scores[winner]

    if depth == 0:
        return 0

    if maxPlayer:
        maxEval = -99999
        for i in range(9):
            if board[i] != " ":
                continue

            board[i] = player
            eval = minmax(board, depth - 1, False)
            board[i] = " "

            if eval > maxEval:
                maxEval = eval

        return maxEval

    else:
        minEval = 99999
        for i in range(9):
            if board[i] != " ":
                continue

            board[i] = opponent
            eval = minmax(board, depth - 1, True)
            board[i] = " "

            if eval < minEval:
                minEval = eval

        return minEval


def main():
    name_str = "NAME:O"
    name = name_str.split(":")[1]
    print(f"Parsed name: {name}")

    move_str = "MOVE:\\ \\ \\ \\ \\ \\ \\ \\ \\ \\"
    move = move_str.split(":")[1]
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

        board[i] = player
        eval = minmax(board, depth - 1, False)
        board[i] = " "
        print(f"Board:{board},Eval:{eval}")

        if eval > maxEval:
            maxEval = eval
            bestMove = i

    print(bestMove)


if __name__ == "__main__":
    main()
