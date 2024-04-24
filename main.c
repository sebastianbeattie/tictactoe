#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

char board_data[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};

char *grey = "\x1b[90m";
char *reset = "\x1b[0m";

char *player_X = "\x1b[1m\x1b[31mX\x1b[0m";
char *player_O = "\x1b[1m\x1b[32mO\x1b[0m";

char *format_mark(int pos)
{
    char mark = board_data[pos];
    char *formatted_pos = (char *)malloc(16 * sizeof(char));
    if (mark == 'X')
    {
        sprintf(formatted_pos, "%s", player_X);
    }
    else if (mark == 'O')
    {
        sprintf(formatted_pos, "%s", player_O);
    }
    else
    {
        sprintf(formatted_pos, "%s%d%s", grey, (pos + 1), reset);
    }
    return formatted_pos;
}

void draw_board()
{
    char *formatted_pos[9];
    for (int i = 0; i < 9; i++)
    {
        formatted_pos[i] = format_mark(i);
    }

    printf("╔═══╦═══╦═══╗\n");
    printf("║ %s ║ %s ║ %s ║\n", formatted_pos[0], formatted_pos[1], formatted_pos[2]);
    printf("╠═══╬═══╬═══╣\n");
    printf("║ %s ║ %s ║ %s ║\n", formatted_pos[3], formatted_pos[4], formatted_pos[5]);
    printf("╠═══╬═══╬═══╣\n");
    printf("║ %s ║ %s ║ %s ║\n", formatted_pos[6], formatted_pos[7], formatted_pos[8]);
    printf("╚═══╩═══╩═══╝\n");

    for (int i = 0; i < 9; i++)
    {
        free(formatted_pos[i]);
    }
}

bool check_if_available(int index)
{
    return board_data[index] == ' ';
}

void make_move(int position_index, char mark)
{
    board_data[position_index] = mark;
}

bool check_win(char current_turn)
{
    for (int i = 0; i < 3; i++)
    {
        if (board_data[i * 3] == current_turn && board_data[i * 3 + 1] == current_turn && board_data[i * 3 + 2] == current_turn)
        {
            return true; // Rows
        }
        if (board_data[i] == current_turn && board_data[i + 3] == current_turn && board_data[i + 6] == current_turn)
        {
            return true; // Columns
        }
    }
    if (board_data[0] == current_turn && board_data[4] == current_turn && board_data[8] == current_turn)
    {
        return true; // Diagonal
    }
    if (board_data[2] == current_turn && board_data[4] == current_turn && board_data[6] == current_turn)
    {
        return true; // Anti-diagonal
    }
    return false;
}

bool check_tie()
{
    for (int i = 0; i < 9; i++)
    {
        if (board_data[i] == ' ')
        {
            return false;
        }
    }
    return true;
}

void find_empty_slots(int *empty_slots, int *empty_slots_size)
{
    *empty_slots_size = 0;
    for (int i = 0; i < 9; i++)
    {
        if (board_data[i] == ' ')
        {
            empty_slots[(*empty_slots_size)++] = i;
        }
    }
}

int minimax(char current_player)
{
    int empty_slots[9];
    int empty_slots_size;
    find_empty_slots(empty_slots, &empty_slots_size);

    if (check_win('X'))
    {
        return -1;
    }
    else if (check_win('O'))
    {
        return 1;
    }
    else if (check_tie())
    {
        return 0;
    }

    int best_score;
    if (current_player == 'O')
    {
        best_score = -700;
        for (int i = 0; i < empty_slots_size; i++)
        {
            int index = empty_slots[i];
            board_data[index] = 'O';
            int score = minimax('X');
            if (score > best_score)
            {
                best_score = score;
            }
            board_data[index] = ' ';
        }
    }
    else
    {
        best_score = 700;
        for (int i = 0; i < empty_slots_size; i++)
        {
            int index = empty_slots[i];
            board_data[index] = 'X';
            int score = minimax('O');
            if (score < best_score)
            {
                best_score = score;
            }
            board_data[index] = ' ';
        }
    }
    return best_score;
}

void clear()
{
    printf("\e[1;1H\e[2J");
}

int main()
{

    char current_turn = 'X';
    int playing = 1;
    while (playing)
    {
        clear();
        if (current_turn == 'X')
        {
            printf("You're playing as %s, the AI is playing as %s\n", player_X, player_O);
            draw_board();

            char input[10];
            int position_index;

            printf("Enter a number between 1 and 9: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0';

            int num = atoi(input);
            if (num < 1 || num > 9)
            {
                printf("Invalid input. Number must be between 1 and 9 inclusive.\n");
                continue;
            }
            position_index = num - 1;
            if (check_if_available(position_index))
            {
                make_move(position_index, current_turn);
                if (check_win(current_turn))
                {
                    draw_board();
                    printf("\nPlayer %c has won this game!\n", current_turn);
                    playing = 0;
                }
                else if (check_tie())
                {
                    draw_board();
                    printf("The game has ended in a tie!\n");
                    playing = 0;
                }
                current_turn = 'O';
            }
            else
            {
                printf("That position isn't available!\n");
            }
        }
        else
        {
            int best_score = -700;
            int best_move;
            for (int i = 0; i < 9; i++)
            {
                if (board_data[i] == ' ')
                {
                    board_data[i] = 'O';
                    int score = minimax('X');
                    if (score > best_score)
                    {
                        best_score = score;
                        best_move = i;
                    }
                    board_data[i] = ' ';
                }
            }
            make_move(best_move, current_turn);
            if (check_win(current_turn))
            {
                draw_board();
                printf("\nPlayer %c has won this game!\n", current_turn);
                playing = 0;
            }
            else if (check_tie())
            {
                draw_board();
                printf("The game has ended in a tie!\n");
                playing = 0;
            }
            current_turn = 'X';
        }
    }
}
