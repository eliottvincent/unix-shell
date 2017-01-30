#include <dirent.h>
#include <sys/stat.h>

#include "divers.h"
#include "commandes_internes.h"
#include "entities.c"


t_bool ActionECHO(parse_info *info, int debut, int nbArg) {

    int i;
    FILE *sortie;

    if (!EST_EGAL(info->sortie, "")) {  //redirection demandée
        sortie = fopen(info->sortie,"w");
        if (sortie == NULL) {   //si le fichier n'est pas accessible en écriture

        }
    }
    else {  //aucune redirection demandée
        sortie = stdout;
    }

    i = 1;
    while (i < nbArg) {
        fprintf(sortie, "%s ", info->ligne_cmd[debut + i]);
        i++;
    }
    printf("\n");

    if (! EST_EGAL(info->sortie, "")) {
        fclose(sortie); //fermeture de la sortie (uniquement pour le fichier)
    }

    return vrai;
}



t_bool ActionSET(parse_info *info, int debut, int nbArg) {
    affiche_parse_info(info);

    if (!EST_EGAL(info->ligne_cmd[debut], "set") && !EST_EGAL(info->ligne_cmd[debut + 2], "=")) {
        DEBUG(printf("Impossible de mettre a jour la variable, appel incorrect\n"));
        return faux;
    }

    if (nbArg == 4) {
        return ecrire_variable(info->ligne_cmd[debut + 1], info->ligne_cmd[debut + 3]);
    }
    else if (nbArg == 3) {
        return ecrire_variable(info->ligne_cmd[debut + 1], NULL);
    }
    else {
        return faux;
    }
}

t_bool ActionCD(parse_info *info, int debut, int nbArg) {

    char def[MAX_PATH];
    char dirName[MAX_PATH];
    int i;

    if (nbArg == 1) {
        lire_variable("HOME", def, sizeof(def));
        if (chdir(def) == -1) {
            DEBUG(printf("Impossible de changer vers le repertoire '%s' \n", def));
            return faux;
        }
    } else {
        strcpy(dirName, "");
        for (i = 1; i < nbArg; i++) {   //aggrégation de tout les arguments
            if (strlen(dirName) != 0) strcat(dirName, " ");
            strcat(dirName, info->ligne_cmd[debut + i]);
        }

        if (chdir(dirName) == -1) {
            DEBUG(printf("Impossible de changer vers le repertoire '%s'\n", dirName));
            return faux;
        }
    }
    return vrai;

}

t_bool ActionLS(parse_info *info, int debut, int nbArg) {

    /* Utilisation des parametres */
    /* Premiere etape */
    (void) info;
    (void) debut;
    (void) nbArg;

    char* folder=".";
    DIR *d;
    struct dirent *dir;
    struct stat st;

    d = opendir(folder);
    if (!d)
        return faux;
    else {
        while ((dir = readdir(d)) != NULL) {
            switch (st.st_mode & S_IFMT) {
                case S_IFREG:
                    printf("regular file || ");
                    break;
                case S_IFDIR:
                    printf("directory || ");
                    break;
                case S_IFCHR:
                    printf("character device || ");
                    break;
                case S_IFBLK:
                    printf("block device || ");
                    break;
                case S_IFLNK:
                    printf("symbolic link || ");
                    break;
                case S_IFIFO:
                    printf("pipe || ");
                    break;
                case S_IFSOCK:
                    printf("socket || ");
                    break;
                default:
                    printf("unknown || ");
            }
            printf("%s\n", dir->d_name);

        }
        closedir(d);
    }
    return vrai;
}

t_bool ActionPWD(parse_info *info, int debut, int nbArg) {
    char wd[MAX];
    wd[MAX-1] = '\0';

    if (getcwd(wd, MAX-1) == NULL) {
        printf("Can not get current working directory\n");
        return faux;
    }
    else {
        printf("%s\n", wd);
        return vrai;
    }
}