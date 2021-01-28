#include "trainer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

void print_header();

int main(void) {
    list *head = initialize();
    list *p_head = head;
    int settings;
    while(TRUE) {
        print_header();
        switch(menue()) {
            case LEAVE:
                save(head);
                puts("Bis zum nächsten Mal.");
                exit(EXIT_SUCCESS);
            case TRAINING:
                trainer(head);
                break;
            case ADD_VOC:
                head = add_vocabulary(head);
                break;
            case PRINT_VOC:
                print_vocabulary(head);
                break;
            default:
                puts("SOMETHING WENT WRONG");
                break;
        }
    clear_screen(0,0);
    }

    return 0;
}

int save(list *head) {
    char buffer[80];
    FILE *fp;
    fp = fopen(DBFILE, "w");
    while(head) {
        strcpy(buffer, head->original);
        strcat(buffer, ";");
        strcat(buffer, head->translation);
        strcat(buffer, ";");
        char c_progress[2];
        sprintf(c_progress, "%d", head->progress);
        strcat(buffer, c_progress);
        strcat(buffer, "\n");
        fputs(buffer, fp);
        head = head->next;
    }
    fclose(fp);
    return 0;
}

void print_header() {
    clear();
    /* PRINT HEADER */
    puts("╔═════════════════════════════════════════════════════════════════╗");
    puts("║   ________                   __                                 ║");
    puts("║  |        \\                 |  \\                                ║");
    puts("║   \\$$$$$$$$______   ______   \\$$ _______    ______    ______    ║");
    puts("║     | $$  /      \\ |      \\ |  \\|       \\  /      \\  /      \\   ║");
    puts("║     | $$ |  $$$$$$\\ \\$$$$$$\\| $$| $$$$$$$\\|  $$$$$$\\|  $$$$$$\\  ║");
    puts("║     | $$ | $$   \\$$/      $$| $$| $$  | $$| $$    $$| $$   \\$$  ║");
    puts("║     | $$ | $$     |  $$$$$$$| $$| $$  | $$| $$$$$$$$| $$        ║");
    puts("║     | $$ | $$      \\$$    $$| $$| $$  | $$ \\$$     \\| $$        ║");
    puts("║      \\$$  \\$$       \\$$$$$$$ \\$$ \\$$   \\$$  \\$$$$$$$ \\$$        ║");
    puts("║                                                                 ║");
    puts("╠═════════════════════════════════════════════════════════════════╣");
}

void clear_screen(int line, int column) {

    /* DELETE THIS OBSOLETE FUNCTION */

    clear();
    /* PRINT HEADER */
    puts("*******************************************************************");
    puts("|   ________                   __                                 |");
    puts("|  |        \\                 |  \\                                |");
    puts("|   \\$$$$$$$$______   ______   \\$$ _______    ______    ______    |");
    puts("|     | $$  /      \\ |      \\ |  \\|       \\  /      \\  /      \\   |");
    puts("|     | $$ |  $$$$$$\\ \\$$$$$$\\| $$| $$$$$$$\\|  $$$$$$\\|  $$$$$$\\  |");
    puts("|     | $$ | $$   \\$$/      $$| $$| $$  | $$| $$    $$| $$   \\$$  |");
    puts("|     | $$ | $$     |  $$$$$$$| $$| $$  | $$| $$$$$$$$| $$        |");
    puts("|     | $$ | $$      \\$$    $$| $$| $$  | $$ \\$$     \\| $$        |");
    puts("|      \\$$  \\$$       \\$$$$$$$ \\$$ \\$$   \\$$  \\$$$$$$$ \\$$        |");
    puts("|                                                                 |");
    puts("*******************************************************************");
}

list *initialize() {
    list *head = NULL;
    list *p_head = NULL;
    list *data = NULL;
    FILE *fp;
    fp = fopen(DBFILE, "ab+");
    char original[80], translation[80], buffer[80];
    int progress;
    while(fgets(buffer, 80, fp) != NULL) {
        data = (list *)malloc(sizeof(list));
        if(data == NULL) {
            puts("Can't allocate memory");
            exit(EXIT_FAILURE);
        }
        char *p_substr = strtok(buffer, ";");
        int i = 0;
        while(p_substr != NULL) {
            if(i == 0) {
                data->original = (char *)malloc(strlen(p_substr)+1);
                if(data->original == NULL) {
                    puts("Can't allocate memory: data->original");
                    exit(EXIT_FAILURE);
                }
                strcpy(data->original, p_substr);
            } else if(i == 1) {
                data->translation = (char *)malloc(strlen(p_substr)+1);
                if(data->translation == NULL) {
                    puts("Can't allocate memory: data->translation");
                    exit(EXIT_FAILURE);
                }
                strcpy(data->translation, p_substr);
            } else if(i==2) {
                char number = p_substr[0];
                data->progress = number - '0';
            }
            p_substr = strtok(NULL, ";");
            i++;
        }

        if(!head) {
            head = data;
            p_head = head;
        } else {
            while(p_head->next) {
                p_head = p_head->next;
            }
            p_head->next = data;
        }

    }
    fclose(fp);

    return head;
}

void dump_stdin() {
    char dump;
    while ((dump = getchar()) != '\n' && dump != EOF);
}

void print_vocabulary(list *head) {
    gotoxy(0,12);
    puts("╠═════════════════════════════════════════════════════════════════╣");
    puts("║                        VOKABELLISTE                             ║");
    puts("╚═════════════════════════════════════════════════════════════════╝");
    int pause = 0;
    puts("----------------------------------------------------------------------");
    printf("| %25s | %25s | %2s |\n", "Original         ", "Übersetzung      ", "Fortschritt");
    puts("----------------------------------------------------------------------");
    dump_stdin();
    while(head) {
        printf("| %25s | %24s | %10d |\n", head->original, head->translation, head->progress);
        //putchar(head->translation[0]);
        head = head->next;
        pause++;
        if(pause == 20) {
            pause = 0;
            puts("Rollen...");
            getc(stdin);
        }
    }
    puts("----------------------------------------------------------------------");
    puts("Taste drücken...");
    getc(stdin);
}


list *add_vocabulary(list *head) {
    char ch, buffer[80];
    list *new = (list *) malloc(sizeof(list));
    if(!new) {
        puts("Can't allocate memory!");
        exit(EXIT_FAILURE);
    }
    new->original = NULL;
    new->translation = NULL;
    new->progress = BEGINNER;
    new->next = NULL;

    puts("Vokabel eingeben");
    printf(">> ");
    while ((ch = getchar()) != '\n' && ch != EOF);
    fgets(buffer, 80, stdin);
    new->original = malloc(strlen(buffer) + 1);
    if(!new->original) {
        puts("Can't allocate memory!");
        exit(EXIT_FAILURE);
    }
    buffer[strcspn(buffer, "\n")] = 0;
    strcpy(new->original, buffer);

    puts("Übersetzung eingeben");
    printf(">> ");
    fgets(buffer, 80, stdin);
    new->translation = malloc(strlen(buffer) + 1);
    if(!new->translation) {
        puts("Can't allocate memory!");
        exit(EXIT_FAILURE);
    }
    buffer[strcspn(buffer, "\n")] = 0;
    strcpy(new->translation, buffer);

    if(!head)
        head = new;
    else {
        list *p_head = head;
        while(p_head->next)
            p_head = p_head->next;
        p_head->next = new;
    }

    return head;
}


void trainer(list *head) {
    char dump;
    char source[80], trans[80], answer[80];
    list *p_head = head;
    int settings, choice;
    int i=0;
    gotoxy(0,12);
    puts("╠═════════════════════════════════════════════════════════════════╣");
    puts("║                                                                 ║");
    puts("║                    TRAINER: Einstellungen                       ║");
    puts("║                                                                 ║");
    puts("╠═════════════════════════════════════════════════════════════════╣");
    puts("║ 1. Ungarisch -> Deutsch                                         ║");
    puts("║ 2. Deutsch -> Ungarisch                                         ║");
    puts("║ 3. Zufällig                                                     ║");
    puts("╠═════════════════════════════════════════════════════════════════╣");
    puts("║ 0. Zurück                                                       ║");
    puts("╚═════════════════════════════════════════════════════════════════╝");
    printf("\033[K>> ");
    scanf("%d", &choice);

    gotoxy(0,12);
    puts("╠═════════════════════════════════════════════════════════════════╣");
    puts("║                                                                 ║");
    puts("║                    TRAINER: Hilfe                               ║");
    puts("║                                                                 ║");
    puts("╠═════════════════════════════════════════════════════════════════╣");
    puts("║ Folgende Befehle sind möglich                                   ║");
    puts("║ -----------------------------                                   ║");
    puts("║ 1. stop: Beendet den Trainer                                    ║");
    puts("║ 2. help: Enthüllt Buchstaben                                    ║");
    puts("╚═════════════════════════════════════════════════════════════════╝");

    /* Store cursor position */
    printf("\033[s");

    while ((dump = getchar()) != '\n' && dump != EOF);
    while(p_head) {
        switch(choice) {
            case 1:
                settings = 1;
                break;
            case 2:
                settings = 2;
                break;
            case 3:
                settings = (rand() % 2) + 1;
                break;
            case 0:
                return;
        }
        /* Clear lines from restored cursor position*/
        printf("\033[u");
        i=0;
        while(i<5) {
            printf("\033[K");
            printf("\n");
            i++;
        }
        printf("\033[u");

        if(p_head->progress == 4) {
            p_head = p_head->next;
            continue;
        }

        if(settings == 1) {
            strcpy(source, p_head->original);
            strcpy(trans, p_head->translation);
        } else {
            strcpy(source, p_head->translation);
            strcpy(trans, p_head->original);
        }

        puts(source);
        printf(">> \033[K");

        i=0;
        char *p_word_len = trans;
        while(*p_word_len != '\0'){
            printf("%c", 95);
            p_word_len++;
            i++;
        }
        printf("\033[%dD", i);
        fgets(answer, 80, stdin);
        answer[strcspn(answer, "\n")] = 0;
        if(!strcmp(answer, "stop")) {
            return;
        } else if(!strcmp(answer, "help")) {
            printf("\033[1A>> ");
            i=0;
            p_word_len = trans;
            while(*p_word_len != '\0'){
                if(i%2)
                    printf("%c",trans[i]);
                else {
                    printf("%c", 95);
                }
                p_word_len++;
                i++;
            }
            getc(stdin);
            continue;
        } else if(strcmp(answer, trans)) {
                puts("Leider falsch.");
                printf("Deine Antwort war: %s\n", answer);
                printf("Die richtige Antwort lautet: %s\n", trans);
                if(p_head->progress>0)
                  p_head->progress--;
        } else {
            puts("RICHTIG!");
            p_head->progress++;
        }
        getc(stdin);
        p_head = p_head->next;
    }
}

int menue() {

    int choice = -1;
    while(choice < 0 | choice > 3) {
        choice = -1;
        puts("║                                                                 ║");
        puts("║                           HAUPTMENUE                            ║");
        puts("║                                                                 ║");
        puts("╠═════════════════════════════════════════════════════════════════╣");
        puts("║ 1. Training starten                                             ║");
        puts("║ 2. Vokabeln hinzufügen                                          ║");
        puts("║ 3. Vokabeln anzeigen                                            ║");
        puts("╠═════════════════════════════════════════════════════════════════╣");
        puts("║ 0. Speichern und Verlassen                                      ║");
        puts("╚═════════════════════════════════════════════════════════════════╝");
        printf(">> ");
        scanf("%d", &choice);
    }

    return choice;
}
