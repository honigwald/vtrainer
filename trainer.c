#include "trainer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    clear_screen(0,0);
    list *head = initialize();
    list *p_head = head;
    int settings;
    while(TRUE) {
        switch(menue()) {
            case LEAVE:
                save(head);
                puts("Bis zum nächsten Mal.");
                exit(EXIT_SUCCESS);
            case TRAINING:
                clear_screen(0,0);
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

void clear_screen(int line, int column) {
    printf("\033[%d;%dH", line, column);

    int width = 0;
    int height = 0;
    while(height < 50) {
        while(width < 80) {
            printf(" ");
            width++;
        }
        printf("\n");
        width = 0;
        height++;
    }
    printf("\033[%d;%dH", line, column);
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

void print_vocabulary(list *head) {
    puts("----------------------------------------------------------------------");
    printf("| %25s | %25s | %2s |\n", "Original         ", "Übersetzung      ", "Fortschritt");
    puts("----------------------------------------------------------------------");
    while(head) {
        printf("| %25s | %24s | %10d |\n", head->original, head->translation, head->progress);
        //putchar(head->translation[0]);
        head = head->next;
    }
    puts("----------------------------------------------------------------------");
    char dump;
    while ((dump = getchar()) != '\n' && dump != EOF);
    char wait = getc(stdin);
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
    char dump, answer[80];
    list *p_head = head;
    int settings, choice;

    puts("|                 TRAINERMENUE: Einstellungen                     |");
    puts("*******************************************************************");
    puts("| 1. Ungarisch -> Deutsch                                         |");
    puts("| 2. Deutsch -> Ungarisch                                         |");
    puts("| 3. Zufällig                                                     |");
    puts("*******************************************************************");
    puts("| 0. Zurück                                                       |");
    puts("*******************************************************************");
    printf(">> ");
    scanf("%d", &choice);


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
                puts("RANDOM");
                break;
            case 0:
                return;
        }
        if(p_head->progress == 4) {
            p_head = p_head->next;
            continue;
        }
        char *p_word_len;
        int i;
        if(settings == 1) {
            puts(p_head->original);
            p_word_len = p_head->translation;
        }
        else {
            puts(p_head->translation);
            p_word_len = p_head->original;
        }
        printf(">> ");

        i=0;
        while(*p_word_len != '\0'){
            printf("%c", 95);
            p_word_len++;
            i++;
        }
        printf("\033[%dD", i);
        fgets(answer, 80, stdin);
        answer[strcspn(answer, "\n")] = 0;
        if(settings == 1) {
            if(strcmp(answer, p_head->translation)) {
                puts("Leider falsch.");
                printf("Die richtige Antwort lautet: %s\n", p_head->translation);
                printf("Deine Antwort war: %s\n", answer);
                if(p_head->progress>0)
                  p_head->progress--;
            }
            else {
                puts("RICHTIG!");
                p_head->progress++;
            }
        } else {
            if(strcmp(answer, p_head->original)) {
                puts("Leider falsch.");
                printf("Die richtige Antwort lautet: %s\n", p_head->original);
                printf("Deine Antwort war: %s\n", answer);
                if(p_head->progress>0)
                  p_head->progress--;
            }
            else {
                puts("RICHTIG!");
                p_head->progress++;
            }
        }
        p_head = p_head->next;
    }
}

int menue() {

    int choice = -1;
    while(choice < 0 | choice > 3) {
        choice = -1;
        puts("|                           HAUPTMENUE                            |");
        puts("*******************************************************************");
        puts("| 1. Training starten                                             |");
        puts("| 2. Vokabeln hinzufügen                                          |");
        puts("| 3. Vokabeln anzeigen                                            |");
        puts("*******************************************************************");
        puts("| 0. Speichern und Verlassen                                      |");
        puts("*******************************************************************");
        printf(">> ");
        scanf("%d", &choice);
    }

    return choice;
}
