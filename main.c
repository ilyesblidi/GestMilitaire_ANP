#include <gtk/gtk.h>
#include <pango/pango-font.h>

#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "fonction.h"
#include "variable_globale.h"
#include "machine_abtraite.h"
#include "affichage.h"

// Function to print the contents of the index1 table
Buffer buffer,buf1 ;
BufferIndexP bufferIndexP;
BufferIndexM bufferIndexM;
Entete header ;
Index indexP ;
IndexM indexM,indexF,indexG;
date currentDate;

int Lecture;
int Ecriture;

/********************** Signal handler to allow only numeric input ******************/
gboolean on_entry_input(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
    // Check if the event is a key press
    if (event->type == GDK_KEY_PRESS) {
        GdkEventKey *key_event = (GdkEventKey *)event;

        // Check if the key is a digit or a backspace
        if ((key_event->keyval >= GDK_KEY_0 && key_event->keyval <= GDK_KEY_9) ||
            key_event->keyval == GDK_KEY_BackSpace) {
            // Allow the input
            return FALSE;
        } else {
            // Block the input
            return TRUE;
        }
    }

    // Allow other events
    return FALSE;
}
/******************************************************************************/

// Define a structure to hold both mainWindow and entryBuffer
typedef struct {
    GtkWidget *mainWindow;
    GtkEntryBuffer *entryBuffer;
    FILE * fichier ;
    FILE * fich ;
    FILE * Fi ;
    FILE * F ;
} UserData;

// Callback function for "Valider" button click
void valider_button_clicked(GtkWidget *widget, gpointer user_data) {

    // Get the user data containing both mainWindow and entryBuffer
    UserData *data = (UserData *)user_data;

    // Get the entered number from the entry buffer
    const char *numberStr = gtk_entry_buffer_get_text(data->entryBuffer);

    // Print the entered number
    g_print("Entered Number: %s\n", numberStr);

    int number = atoi(numberStr);

    /** chargement_initiale(f,number); **/
    Lecture = 0;
    Ecriture = 0;

    chargementInitial(data->fichier, number);

    printf("le cout de chargement initial est %d lecture et %d ecriture\n", Lecture, Ecriture);
    printfFichier("PERSONNEL-ANP_DZ.dat");
    printf("test\n");
    /***********************************/

    // Add your logic here using mainWindow and entryBuffer as needed

    GtkWidget *warningDialog = gtk_message_dialog_new(GTK_WINDOW(data->mainWindow),
                                                      GTK_DIALOG_MODAL,
                                                      GTK_MESSAGE_WARNING,
                                                      GTK_BUTTONS_OK,
                                                      "Chargement initiale fait avec succès");
    gtk_dialog_run(GTK_DIALOG(warningDialog));
    gtk_widget_destroy(warningDialog);

    Fermer(data->fichier);
    Sauvegarde_IndexM(data->fich, indexM);
            Sauvegarde_IndexM(data->Fi, indexF);
            Sauvegarde_IndexM(data->F, indexG);
            Fermer(data->fich);
            Fermer(data->Fi);
            Fermer(data->F);
//            printGradeIndex("GRADE_INDEX.idx");
//            printMilitaireIndex("MILITAIRE_INDEX.idx");
//            printForceIndex("FORCE_ARME_INDEX.idx");
    // Clean up the allocated user data
    g_free(data);
}

// Function to create content for Button 1
void create_button1_content(GtkWidget *mainWindow, const char *buttonLabel) {

    FILE *fich, *Fi, *F, *fichier;

    Ouvrire(&fichier, "PERSONNEL-ANP_DZ.dat", 'N');
            Ouvrire(&fich, "MILITAIRE_INDEX.idx", 'N');
            Ouvrire(&Fi, "FORCE_ARME_INDEX.idx", 'N');
            Ouvrire(&F, "GRADE_INDEX.idx", 'N');


    // Create a new window for Button 1 content
    GtkWidget *buttonWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(buttonWindow), buttonLabel);
    gtk_window_set_default_size(GTK_WINDOW(buttonWindow), 300, 100);
    gtk_window_set_position(GTK_WINDOW(buttonWindow), GTK_WIN_POS_CENTER);

    // Create a box to hold the content in the new window
    GtkWidget *buttonBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(buttonWindow), buttonBox);

    // Create a label for Button 1 content
    GtkWidget *buttonLabelWidget = gtk_label_new("Entrer le nombre d'enregistrement");
    gtk_box_pack_start(GTK_BOX(buttonBox), buttonLabelWidget, FALSE, FALSE, 15);

    // Create an entry buffer for the number
    GtkEntryBuffer *entryBuffer = gtk_entry_buffer_new(NULL, 0);

    // Create an entry widget for the number
    GtkWidget *entryNumber = gtk_entry_new_with_buffer(entryBuffer);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entryNumber), "Enter a number");

    g_signal_connect(entryNumber, "key-press-event", G_CALLBACK(on_entry_input), NULL);

    // Create a "Valider" button
    GtkWidget *validerButton = gtk_button_new_with_label("Valider");

    // Create user data structure and set its values
    UserData *userData = g_new(UserData, 1);
    userData->mainWindow = buttonWindow;
    userData->entryBuffer = entryBuffer;
    userData->fichier = fichier ;
    userData->fich = fich ;
    userData->Fi = Fi ;
    userData->F = F ;

    // Connect the callback function to the "clicked" signal of the "Valider" button
    g_signal_connect(validerButton, "clicked", G_CALLBACK(valider_button_clicked), userData);

    // Pack widgets into the box
    gtk_box_pack_start(GTK_BOX(buttonBox), entryNumber, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(buttonBox), validerButton, FALSE, FALSE, 5);

    // Show all widgets in the new window
    gtk_widget_show_all(buttonWindow);
}

// Function to create content for Button 2
void create_button2_content(GtkWidget *mainWindow, const char *buttonLabel) {

            Sauvegarde_Index(indexP);

    // Show a warning dialog when Button 5 is clicked
    GtkWidget *warningDialog = gtk_message_dialog_new(GTK_WINDOW(mainWindow),
                                                      GTK_DIALOG_MODAL,
                                                      GTK_MESSAGE_WARNING,
                                                      GTK_BUTTONS_OK,
                                                      "Sauvegarde de l'index fait avec succès");

    gtk_dialog_run(GTK_DIALOG(warningDialog));
    gtk_widget_destroy(warningDialog);
}

// Function to create content for Button 3
void create_button3_content(GtkWidget *mainWindow, const char *buttonLabel) {

        Chargement_index(&indexP);
        affichIndexPrimaire(indexP);

        // Show a warning dialog
        GtkWidget *warningDialog = gtk_message_dialog_new(GTK_WINDOW(mainWindow),
                                                          GTK_DIALOG_MODAL,
                                                          GTK_MESSAGE_WARNING,
                                                          GTK_BUTTONS_OK,
                                                          "Chargement de l'index fait avec succès");

        gtk_dialog_run(GTK_DIALOG(warningDialog));
        gtk_widget_destroy(warningDialog);

    }

// Define a structure to hold both mainWindow and entryBuffer
typedef struct {
    GtkWidget *mainWindow;
    GtkWidget *searchEntry;
} Userdata;

// Define resultWindow globally so it persists across multiple searches
GtkWidget *resultWindow = NULL;

// Function to add a label to the list box
void add_label_to_list_box(GtkWidget *listBox, const char *linePrefix, const char *lineText, const char *prefixColor, const char *textColor) {
    // Create a grid to hold the label and set its border width
    GtkWidget *grid = gtk_grid_new();
    gtk_container_set_border_width(GTK_CONTAINER(grid), 5);

    // Create a label for linePrefix with black color
    GtkWidget *prefixLabel = gtk_label_new(linePrefix);
    gtk_label_set_markup(GTK_LABEL(prefixLabel), g_markup_printf_escaped("<span foreground='%s'>%s</span>", prefixColor, linePrefix));
    gtk_widget_set_halign(prefixLabel, GTK_ALIGN_START);

    // Create a label for lineText with brown color
    GtkWidget *textLabel = gtk_label_new(lineText);
    gtk_label_set_markup(GTK_LABEL(textLabel), g_markup_printf_escaped("<span foreground='%s'>%s</span>", textColor, lineText));
    gtk_widget_set_halign(textLabel, GTK_ALIGN_START);

    // Attach labels to the grid
    gtk_grid_attach(GTK_GRID(grid), prefixLabel, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), textLabel, 1, 0, 1, 1);

    // Add the grid to the list box
    gtk_container_add(GTK_CONTAINER(listBox), grid);

    // Show all widgets
    gtk_widget_show_all(grid);
}

void date_to_string(const date *date, char *output, size_t size) {
    snprintf(output, size, "%04d-%02d-%02d", date->year, date->month, date->day);
}

// Callback function for "Valider" button click
void search_button_clicked(GtkWidget *widget, gpointer userdata) {

    Userdata *data = (Userdata *)userdata;

    // Get the entered text from the search entry
    const char *searchText = gtk_entry_get_text(GTK_ENTRY(data->searchEntry));
    // Convert the entered text to an integer
    int enteredNumber = atoi(searchText);

    FILE *fichier , *fich ;
    Ouvrire(&fichier, "PERSONNEL-ANP_DZ.dat", 'A');
    Ouvrire(&fich, "MILITAIRE_INDEX.idx", 'A');
    Chargement_index(&indexP);
    Chargement_indexM(fich, &indexM);
            Lecture = 0;
            Ecriture = 0;

    int i,j;
    bool trouv;
    Buffer buf_rech = *alloc_bloc(fichier);

    Rech_Militaires(enteredNumber , &trouv , &i , &j , fichier);
    printf("\n resultat de rech : i=%d ; j=%d",i,j);

    LireDir(fichier,i,&buf_rech);
    //printf("\nLirDir = %d",LireDir(fichier,i,&buf_rech));

    if(!trouv){
        GtkWidget *warningDialog = gtk_message_dialog_new(GTK_WINDOW(data->mainWindow),
                                                          GTK_DIALOG_MODAL,
                                                          GTK_MESSAGE_WARNING,
                                                          GTK_BUTTONS_OK,
                                                          "le militaire que vous recherchez n'exsite pas");

        gtk_dialog_run(GTK_DIALOG(warningDialog));
        gtk_widget_destroy(warningDialog);

    } else {

        resultWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(resultWindow), "Result Window");
        gtk_window_set_default_size(GTK_WINDOW(resultWindow), 500, 500);
        gtk_window_set_position(GTK_WINDOW(resultWindow), GTK_WIN_POS_CENTER);

        // Set the background color of the result window
        GdkRGBA backgroundColor;
        gdk_rgba_parse(&backgroundColor, "#ECECEC");  // Replace "your_color_here" with your desired color
        gtk_widget_override_background_color(resultWindow, GTK_STATE_FLAG_NORMAL, &backgroundColor);

        // Clear previous content of the window
        //gtk_container_foreach(GTK_CONTAINER(resultWindow), (GtkCallback)gtk_widget_destroy, NULL);

/******************************** récuperer les 9 champs de enreg *******************************/
        char labelTextMatricule[7] ;
        snprintf(labelTextMatricule, sizeof(labelTextMatricule), "%d", enteredNumber); ///convertir int to string
        char labelTextNom[31];
        strcpy(labelTextNom,buf_rech.tab[j].Nom);
        char labelTextPrenom[31];
        strcpy(labelTextPrenom,buf_rech.tab[j].Prenom);

        char labelNomPrenom[64] = "";
        strcat(labelNomPrenom,labelTextNom);
        strcat(labelNomPrenom,"  ");
        strcat(labelNomPrenom,labelTextPrenom);

        date SelectedDate = {buf_rech.tab[j].Date_Naissance.day, buf_rech.tab[j].Date_Naissance.month, buf_rech.tab[j].Date_Naissance.year};
        // Convert Date to string
        char labelTextDN[11];
        date_to_string(&SelectedDate, labelTextDN, sizeof(labelTextDN));

        char labelTextWN[32];
        strcpy(labelTextWN,buf_rech.tab[j].Wilaya_Naissance);
        char labelTextGrade[50];
        strcpy(labelTextGrade,buf_rech.tab[j].Grade);
        char labelTextRegion[30];
        strcpy(labelTextRegion,buf_rech.tab[j].Region_militaire);
        char labelTextForce[50];
        strcpy(labelTextForce,buf_rech.tab[j].Force_armee);
        char labelTextGrpS[4];
        strcpy(labelTextGrpS,buf_rech.tab[j].Groupe_sanguin);
        printf("\n(%d) : buf_rech.tab[j].Region : %s ",buf_rech.tab[j].Matricule,buf_rech.tab[j].Region_militaire);

/*********************************************************************************************/

        /// strcat(Dest,Source) : Concatener les 2 chaines et mettre le resultat dans Dest

        // Create a grid to center the list box
        GtkWidget *resultGrid = gtk_grid_new();
        gtk_container_add(GTK_CONTAINER(resultWindow), resultGrid);

        // Create a list box to hold the content in the new window
        GtkWidget *resultListBox = gtk_list_box_new();
        PangoFontDescription *font_desc000 = pango_font_description_from_string("Source Sans Pro 15"); //* Britannic 17 *//
        gtk_widget_override_font(resultListBox, font_desc000);

        gtk_grid_attach(GTK_GRID(resultGrid), resultListBox, 0, 0, 1, 1);

        // Create a label for each line and add them to the list box
        add_label_to_list_box(resultListBox, "  Le militaire ayant le matricule ", labelTextMatricule , "black", "brown" );
        add_label_to_list_box(resultListBox, "  se nomme ", labelNomPrenom , "black", "brown");
        add_label_to_list_box(resultListBox, "  Né le ", labelTextDN , "black", "brown");
        add_label_to_list_box(resultListBox, "  À ", labelTextWN , "black", "brown");
        add_label_to_list_box(resultListBox, "  il est rattaché à la région militaire ", labelTextRegion ,"black", "brown");
        add_label_to_list_box(resultListBox, "  Son grade est ", labelTextGrade , "black", "brown");
        add_label_to_list_box(resultListBox, "  son groupe sanguin est ", labelTextGrpS , "black", "brown");
        add_label_to_list_box(resultListBox, "  et il fait partie de la force armée ", labelTextForce , "black", "brown");

        // Set centering options for the grid
        gtk_widget_set_halign(resultGrid, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(resultGrid, GTK_ALIGN_CENTER);

        // Don't forget to free the font description when you're done
        pango_font_description_free(font_desc000);

        // Show all widgets in the new window
        gtk_widget_show_all(resultWindow);
    }

    printf("le cout de recherche par matricule est %d lecture et %d ecriture\n", Lecture, Ecriture);
    Fermer(fichier);
    Fermer(fich);
}

// Function to create content for Button 4
void create_button4_content(GtkWidget *mainWindow, const char *buttonLabel) {

    // Create a new window for Button 4 content
    GtkWidget *buttonWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(buttonWindow), buttonLabel);
    gtk_window_set_default_size(GTK_WINDOW(buttonWindow), 350, 150);
    gtk_window_set_position(GTK_WINDOW(buttonWindow), GTK_WIN_POS_CENTER);

    // Create a box to hold the content in the new window
    GtkWidget *buttonBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(buttonWindow), buttonBox);

    // Create a label or other content for Button 4
    GtkWidget *buttonLabelWidget = gtk_label_new("Entrer le matricule de militaire recherché");
    gtk_box_pack_start(GTK_BOX(buttonBox), buttonLabelWidget, FALSE, FALSE, 20);

    // Create a search entry
    GtkWidget *searchEntry = gtk_search_entry_new();
    GtkEntryBuffer *searchEntryBuffer = gtk_entry_get_buffer(GTK_ENTRY(searchEntry));
    gtk_entry_buffer_set_max_length(searchEntryBuffer, 6);  // Set maximum length to 6 characters

    // Connect the signal handler to the "key-press-event" signal
    g_signal_connect( searchEntry , "key-press-event", G_CALLBACK(on_entry_input), NULL);


    gtk_box_pack_start(GTK_BOX(buttonBox), searchEntry, FALSE, FALSE, 0);

    // Create a "Valider" button
    GtkWidget *validerButton = gtk_button_new_with_label("SEARCH");

    // Create user data structure and set its values
    Userdata *userdata = g_new(Userdata, 1);
    userdata->mainWindow = buttonWindow;
    userdata->searchEntry = searchEntry ;

    // Connect the callback function to the "clicked" signal of the "Valider" button
    g_signal_connect(validerButton, "clicked", G_CALLBACK(search_button_clicked), userdata);

    // Pack widgets into the box
    gtk_box_pack_start(GTK_BOX(buttonBox), validerButton, FALSE, FALSE, 5);

    // Show all widgets in the new window
    gtk_widget_show_all(buttonWindow);

}

// Define a structure to hold the user data for Button 5
typedef struct {
    GtkWidget *mainWindow;
    GtkEntryBuffer *entryBufferMatricule;
    GtkEntryBuffer *entryBufferNom;
    GtkEntryBuffer *entryBufferPrenom;
    GtkWidget *selectedOptions[5];  // Array to store selected options from combo boxes
    GtkWidget *datePicker;  // Date picker widget
} UserDataButton5;

// Callback function for "VALIDER" button click in Button 5
void valider_button5_clicked(GtkWidget *widget, gpointer user_data) {

    // Get the user data containing mainWindow and entry buffers
    UserDataButton5 *data = (UserDataButton5 *)user_data;

    // Get the entered data from the entry buffers
    const char *matricule = gtk_entry_buffer_get_text(data->entryBufferMatricule);
    int numero_matricule = atoi(matricule);
    const char *nom = gtk_entry_buffer_get_text(data->entryBufferNom);
    const char *prenom = gtk_entry_buffer_get_text(data->entryBufferPrenom);
    const char *wilaya = gtk_combo_box_text_get_active_text((GtkComboBoxText *) data->selectedOptions[0]);
    const char *grp_sanguaine = gtk_combo_box_text_get_active_text((GtkComboBoxText *) data->selectedOptions[1]);
    const char *grade = gtk_combo_box_text_get_active_text((GtkComboBoxText *) data->selectedOptions[2]);
    const char *force = gtk_combo_box_text_get_active_text((GtkComboBoxText *) data->selectedOptions[3]);
    const char *region = gtk_combo_box_text_get_active_text((GtkComboBoxText *) data->selectedOptions[4]);

    // Get the selected date from the date picker
    GtkCalendar *calendar = GTK_CALENDAR(data->datePicker);
    guint year, month, day;
    gtk_calendar_get_date(calendar, &year, &month, &day);

    // Create a GDate structure and set the selected date
    struct date selectedDate;
    selectedDate.year = year;
    selectedDate.month = month + 1;  // Adjust for zero-based month in GtkCalendar
    selectedDate.day = day;

    //printf("\nday : %d ; month : %d ; year : %d ",selectedDate.day,selectedDate.month,selectedDate.year);

    /** ICI ON AJOUTE : Inser_enreg(enreg,Fd); **/
    struct Tenrg e ;
    e.Matricule = numero_matricule;
    strcpy(e.Nom , nom);
    strcpy(e.Prenom , prenom);
    printf("\n nom : %s ; prenom : %s",e.Nom , e.Prenom);
    strcpy(e.Wilaya_Naissance , wilaya);
    printf("\n Wilaya : %s ",e.Wilaya_Naissance);
    strcpy(e.Region_militaire , region);
    printf("\n Region : %s ",e.Region_militaire);
    strcpy(e.Groupe_sanguin , grp_sanguaine);
    strcpy(e.Grade , grade);
    strcpy(e.Force_armee , force);
    e.Date_Naissance = selectedDate ;

    FILE *fichier ,*fich ,*Fi ,*F ;
    bool trouve;

    Ouvrire(&fichier, "PERSONNEL-ANP_DZ.dat", 'A');
    Ouvrire(&fich, "MILITAIRE_INDEX.idx", 'A');
    Ouvrire(&Fi, "FORCE_ARME_INDEX.idx", 'A');
    Ouvrire(&F, "GRADE_INDEX.idx", 'A');
    Chargement_indexM(fich, &indexM);
    Chargement_indexM(Fi, &indexF);
    Chargement_indexM(F, &indexG);
    Chargement_index(&indexP);
    Lecture = 0;Ecriture = 0;
    insertion(fichier, e, &trouve, &indexP, &indexM, &indexF, &indexG);

    if(trouve == false){
        GtkWidget *warningDialog = gtk_message_dialog_new(GTK_WINDOW(data->mainWindow),
                                                          GTK_DIALOG_MODAL,
                                                          GTK_MESSAGE_WARNING,
                                                          GTK_BUTTONS_OK,
                                                          "Insertion fait avec succès");
        gtk_dialog_run(GTK_DIALOG(warningDialog));
        gtk_widget_destroy(warningDialog);

    } else {
        GtkWidget *warningDialog = gtk_message_dialog_new(GTK_WINDOW(data->mainWindow),
                                                          GTK_DIALOG_MODAL,
                                                          GTK_MESSAGE_WARNING,
                                                          GTK_BUTTONS_OK,
                                                          "l'enregistrement que vous voulez inserer existe deja");
        gtk_dialog_run(GTK_DIALOG(warningDialog));
        gtk_widget_destroy(warningDialog);

    }

    printf("le cout de insertion est %d lecture et %d ecriture\n", Lecture, Ecriture);
    //affichIndexPrimaire(indexP);
    //affichIndexMilitaire(indexM);
    printf("le matricule %d\n", e.Matricule);
    Sauvegarde_IndexM(fich, indexM);
    Sauvegarde_IndexM(Fi, indexF);
    Sauvegarde_IndexM(F, indexG);
    Sauvegarde_Index(indexP);
    printfFichier("");
    Fermer(fichier);
    Fermer(fich);
    Fermer(Fi);
    Fermer(F);

    // Clean up the allocated user data
    g_free(data);
}

// Function to create content for Button 5
void create_button5_content(GtkWidget *mainWindow, const char *buttonLabel) {
// Create a new window for Button 5 content
    GtkWidget *buttonWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(buttonWindow), buttonLabel);
    gtk_window_set_default_size(GTK_WINDOW(buttonWindow), 700, 700);
    gtk_window_set_position(GTK_WINDOW(buttonWindow), GTK_WIN_POS_CENTER);

    // Create a box to hold the content in the new window
    GtkWidget *buttonBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_size_request(buttonBox, 300, -1); // Set width to 300
    gtk_container_add(GTK_CONTAINER(buttonWindow), buttonBox);

    // Create a label for the information
    GtkWidget *infoLabel = gtk_label_new("Entrer les informations de militaire");
    PangoFontDescription *font_desc_label = pango_font_description_new();
    pango_font_description_set_weight(font_desc_label, PANGO_WEIGHT_BOLD);
    gtk_widget_override_font(infoLabel, font_desc_label);

    // Create a PangoFontDescription for Algerian font with size 12
    PangoFontDescription *fontDesc = pango_font_description_from_string("Algerian 12");

// Apply the font description to a GTK widget (e.g., a label)
    gtk_widget_override_font(infoLabel, fontDesc);

// Don't forget to free the font description when you're done
    pango_font_description_free(fontDesc);

    gtk_box_pack_start(GTK_BOX(buttonBox), infoLabel, FALSE, FALSE, 15);

    // Create an HBox for the first entry buffer (Matricule)
    GtkWidget *hboxMatricule = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);

    // Create a label for Matricule
    GtkWidget *labelMatricule = gtk_label_new("Matricule :");
    gtk_box_pack_start(GTK_BOX(hboxMatricule), labelMatricule, FALSE, FALSE, 5);

    // Create a PangoFontDescription for Algerian font with size 12
    PangoFontDescription *fontDesc2 = pango_font_description_from_string("Algerian 12");

// Apply the font description to a GTK widget (e.g., a label)
    gtk_widget_override_font(labelMatricule, fontDesc2);

// Don't forget to free the font description when you're done
    pango_font_description_free(fontDesc2);

    // Create the entry buffer for Matricule
    GtkEntryBuffer *entryBufferMatricule = gtk_entry_buffer_new(NULL, 0);
    gtk_entry_buffer_set_max_length(entryBufferMatricule, 6);  // Set maximum length to 6 characters
    GtkWidget *entryMatricule = gtk_entry_new_with_buffer(entryBufferMatricule);
    gtk_widget_set_size_request(entryMatricule, 300, -1); // Set width to 300
    gtk_box_pack_start(GTK_BOX(hboxMatricule), entryMatricule, FALSE, FALSE, 5);

    g_signal_connect(entryMatricule, "key-press-event", G_CALLBACK(on_entry_input), NULL);

    gtk_box_pack_start(GTK_BOX(buttonBox), hboxMatricule, FALSE, FALSE, 5);

    // Create an HBox for the second entry buffer (Nom and Prenom)
    GtkWidget *hboxNomPrenom = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);

    // Create a label for Nom
    GtkWidget *labelNom = gtk_label_new("Nom :");
    gtk_box_pack_start(GTK_BOX(hboxNomPrenom), labelNom, FALSE, FALSE, 5);

    // Create the entry buffer for Nom
    GtkEntryBuffer *entryBufferNom = gtk_entry_buffer_new(NULL, 0);
    gtk_entry_buffer_set_max_length(entryBufferNom, 30);  // Set maximum length to 30 characters
    GtkWidget *entryNom = gtk_entry_new_with_buffer(entryBufferNom);
    gtk_widget_set_size_request(entryNom, 145, -1); // Set width to 145
    gtk_box_pack_start(GTK_BOX(hboxNomPrenom), entryNom, FALSE, FALSE, 5);

    // Create a label for Prenom
    GtkWidget *labelPrenom = gtk_label_new("Prenom :");
    gtk_box_pack_start(GTK_BOX(hboxNomPrenom), labelPrenom, FALSE, FALSE, 5);

    // Create the entry buffer for Prenom
    GtkEntryBuffer *entryBufferPrenom = gtk_entry_buffer_new(NULL, 0);
    gtk_entry_buffer_set_max_length(entryBufferPrenom, 30);  // Set maximum length to 30 characters
    GtkWidget *entryPrenom = gtk_entry_new_with_buffer(entryBufferPrenom);
    gtk_widget_set_size_request(entryPrenom, 145, -1); // Set width to 145
    gtk_box_pack_start(GTK_BOX(hboxNomPrenom), entryPrenom, FALSE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(buttonBox), hboxNomPrenom, FALSE, FALSE, 5);

    // Create an HBox for the date picker
    GtkWidget *hboxDatePicker = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);

    // Create a label for the date picker
    GtkWidget *labelDatePicker = gtk_label_new("Date :");
    gtk_box_pack_start(GTK_BOX(hboxDatePicker), labelDatePicker, FALSE, FALSE, 5);

    // Create the date picker
    GtkWidget *datePicker = gtk_calendar_new();
    gtk_widget_set_size_request(datePicker, 300, -1); // Set width to 300
    gtk_box_pack_start(GTK_BOX(hboxDatePicker), datePicker, FALSE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(buttonBox), hboxDatePicker, FALSE, FALSE, 5);

    //GDate dateselected = gtk_calendar_get_date(GTK_CALENDAR(datePicker)) ;

    // Create an HBox for each combo box
    const char *options[][59] = {
            {"Wilaya de naissance :", "Adrar", "Chlef", "Laghouat", "Oum_El_Bouaghi", "Batna", "Bejaïa", "Biskra",
                    "Bechar", "Blida", "Bouira", "Tamanrasset", "Tebessa", "Tlemcen", "Tiaret",
                    "Tizi_Ouzou", "Alger", "Djelfa", "Jijel", "Setif", "Saïda", "Skikda", "Sidi_Bel_Abbès",
                    "Annaba", "Guelma", "Constantine", "Medea", "Mostaganem", "M'Sila", "Mascara", "Ouargla",
                    "Oran", "El_Bayadh", "Illizi", "Bordj_Bou_Arreridj", "Boumerdès", "El_Tarf", "Tindouf",
                    "Tissemsilt", "El_Oued", "Khenchela", "Souk_Ahras", "Tipaza", "Mila", "Aïn_Defla", "Naâma",
                    "Aïn_Temouchent", "Ghardaïa", "Relizane", "El_M'ghair", "El Menia", "Ouled Djellal",
                    "Bordj Badji Mokhtar", "Béni Abbès", "Timimoun", "Touggourt", "Djanet", "In Salah", "In Guezzam"},
            {"Groupe Sanguin :", "O+", "A+", "B+", "O-", "A-", "AB+", "B-", "AB-"},
            {"Grade :", "General d'Armee", "General de corps d’armee", "General-Major",
                    "General", "Colonel", "Lieutenant-colonel", "Commandant",
                    "Capitaine", "Lieutenant", "Sous-lieutenant", "Aspirant",
                    "Adjudant-Major", "Adjudant-Chef", "Adjudant", "Sergent-Chef",
                    "Sergent", "Caporal-Chef", "Caporal", "Djoundi"},
            {"Force Armée :", "Armee_de_terre", "Armee_de_l’air", "Marine_nationale",
                    "Defense_aerienne_du_territoire", "Gendarmerie_nationale",
                    "Garde_republicaine", "Departement_du_renseignement_et_de_la_securite",
                    "Sante_militaire"},
            {"Région militaire :", "1RM-Blida", "2RM-Oran", "3RM-Béchar", "4RM-Ouargla", "5RM-Constantine", "6RM-Tamanrasset"}
    };

    // Declare UserDataButton5 structure
    UserDataButton5 *userData = g_new(UserDataButton5, 1);

    // Create an array to store combo boxes
    GtkWidget *combo_boxes[5];

    for (int i = 0; i < 5; ++i) {
        GtkWidget *hboxComboBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);

        // Create a label for the combo box
        GtkWidget *labelComboBox = gtk_label_new(options[i][0]);
        gtk_box_pack_start(GTK_BOX(hboxComboBox), labelComboBox, FALSE, FALSE, 10);

        // Create the combo box
        GtkWidget *comboBox = gtk_combo_box_text_new();

        // Add options based on the array for the current combo box
        for (int j = 1; j < sizeof(options[i]) / sizeof(options[i][0]); ++j) {
            if (options[i][j] != NULL) {
                gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboBox), options[i][j]);
            } else {
                break; // Stop adding options if NULL is encountered
            }
        }

        // Store the combo box in the array
        combo_boxes[i] = comboBox;

        gtk_widget_set_size_request(comboBox, 300, -1); // Set width to 300
        gtk_box_pack_start(GTK_BOX(hboxComboBox), comboBox, FALSE, FALSE, 5);

        gtk_box_pack_start(GTK_BOX(buttonBox), hboxComboBox, FALSE, FALSE, 5);
    }

    // Create a "VALIDER" button
    GtkWidget *validerButton = gtk_button_new_with_label("VALIDER");

    // Set values for userData
    userData->mainWindow = buttonWindow;
    userData->entryBufferMatricule = entryBufferMatricule;
    userData->entryBufferNom = entryBufferNom;
    userData->entryBufferPrenom = entryBufferPrenom;

    // Store the combo boxes in UserDataButton5
    for (int i = 0; i < 5; ++i) {
        userData->selectedOptions[i] = combo_boxes[i];
    }

    // Store the date picker in UserDataButton5
    userData->datePicker = datePicker;

    // Connect the callback function to the "clicked" signal of the "VALIDER" button
    g_signal_connect(validerButton, "clicked", G_CALLBACK(valider_button5_clicked), userData);

    // Pack the "VALIDER" button into the box
    gtk_box_pack_start(GTK_BOX(buttonBox), validerButton, FALSE, FALSE, 5);

    // Show all widgets in the new window
    gtk_widget_show_all(buttonWindow);
}

// Callback function for "Valider" button click
void supprimer_button_clicked(GtkWidget *widget, gpointer user_data) {
    // Get the user data containing both mainWindow and entryBuffer
    UserData *data = (UserData *)user_data;

    // Get the entered number from the entry buffer
    const char *number = gtk_entry_buffer_get_text(data->entryBuffer);
    int cle = atoi(number) ;

    /** supprimer_enreg(fichier: Fd , Matricule: number); **/
    FILE *fic ,*fich ,*Fi ,*F ;
    bool trouve;

    Chargement_index(&indexP);
    Ouvrire(&fic,"PERSONNEL-ANP_DZ.dat",'A');
    Ouvrire(&Fi,"MILITAIRE_INDEX.idx",'A');
    Ouvrire(&fich,"FORCE_ARME_INDEX.idx",'A');
    Ouvrire(&F,"GRADE_INDEX.idx",'A');
    Chargement_indexM(Fi,&indexM);
    Chargement_indexM(fich,&indexF);
    Chargement_indexM(F,&indexG);
    printf("***************avant*********************\n");
    printfFichier("PERSONNEL-ANP_DZ.dat");
    Lecture =0;Ecriture = 0;
    suppersionEnrg(fic,cle,&trouve,&indexP);

    if(trouve == true){
        GtkWidget *warningDialog = gtk_message_dialog_new(GTK_WINDOW(data->mainWindow),
                                                          GTK_DIALOG_MODAL,
                                                          GTK_MESSAGE_WARNING,
                                                          GTK_BUTTONS_OK,
                                                          "Suppression d'enregistrement fait avec succès");
        gtk_dialog_run(GTK_DIALOG(warningDialog));
        gtk_widget_destroy(warningDialog);

    } else {
        GtkWidget *warningDialog = gtk_message_dialog_new(GTK_WINDOW(data->mainWindow),
                                                          GTK_DIALOG_MODAL,
                                                          GTK_MESSAGE_WARNING,
                                                          GTK_BUTTONS_OK,
                                                          "l'enregistrement que vous voulez supprimer n'existe pas");
        gtk_dialog_run(GTK_DIALOG(warningDialog));
        gtk_widget_destroy(warningDialog);
    }

    printf("le cout de suppression est %d lecture et %d ecriture\n",Lecture,Ecriture);
    Sauvegarde_Index(indexP);
    Sauvegarde_IndexM(Fi,indexM);
    Sauvegarde_IndexM(fich,indexF);
    Sauvegarde_IndexM(F,indexG);
    Fermer(fic);
    Fermer(Fi);
    Fermer(fich);
    Fermer(F);
    printf("***************apres*********************\n");
    printfFichier("PERSONNEL-ANP_DZ.dat");
    printf("***************apres*********************\n");
    printIndexPrimaire("MATRICULE_INDEX.idx");
    printf("******************************************\n");
    printMilitaireIndex("MILITAIRE_INDEX.idx");
    printf("***************apres*********************\n");
    printForceIndex("FORCE_ARME_INDEX.idx");
    printf("***************apres*********************\n");
    printGradeIndex("GRADE_INDEX.idx");

    // Clean up the allocated user data
    g_free(data);
}

// Function to create content for Button 6
void create_button6_content(GtkWidget *mainWindow, const char *buttonLabel) {

    // Create a new window for Button 1 content
    GtkWidget *buttonWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(buttonWindow), buttonLabel);
    gtk_window_set_default_size(GTK_WINDOW(buttonWindow), 300, 100);
    gtk_window_set_position(GTK_WINDOW(buttonWindow), GTK_WIN_POS_CENTER);

    // Create a box to hold the content in the new window
    GtkWidget *buttonBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(buttonWindow), buttonBox);

    // Create a label for Button 1 content
    GtkWidget *buttonLabelWidget = gtk_label_new("   Entrer le matricule d'enregistrement que vous voulez supprimer   ");
    gtk_box_pack_start(GTK_BOX(buttonBox), buttonLabelWidget, FALSE, FALSE, 15);

    // Create an entry buffer for the number
    GtkEntryBuffer *entryBuffer = gtk_entry_buffer_new(NULL, 0);
    gtk_entry_buffer_set_max_length(entryBuffer, 6);  // Set maximum length to 6 characters

    // Create an entry widget for the number
    GtkWidget *entryNumber = gtk_entry_new_with_buffer(entryBuffer);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entryNumber), "Enter a number");

    g_signal_connect(entryNumber, "key-press-event", G_CALLBACK(on_entry_input), NULL);

    // Create a "Valider" button
    GtkWidget *validerButton = gtk_button_new_with_label("Supprimer");

    // Create user data structure and set its values
    UserData *userData = g_new(UserData, 1);
    userData->mainWindow = buttonWindow;
    userData->entryBuffer = entryBuffer;

    // Connect the callback function to the "clicked" signal of the "Valider" button
    g_signal_connect(validerButton, "clicked", G_CALLBACK(supprimer_button_clicked), userData);

    // Pack widgets into the box
    gtk_box_pack_start(GTK_BOX(buttonBox), entryNumber, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(buttonBox), validerButton, FALSE, FALSE, 5);

    // Show all widgets in the new window
    gtk_widget_show_all(buttonWindow);
}

typedef struct {
    GtkWidget *mainWindow;
    GtkEntryBuffer *entryBuffer;
    GtkComboBox *regionComboBox;
} UserData2;

// Callback function for "Valider" button click
void modifier_button_clicked(GtkWidget *widget, gpointer user_data) {
    // Get the user data containing both mainWindow and entryBuffer
    UserData2 *data = (UserData2 *)user_data;

    // Get the entered number from the entry buffer
    const char *number = gtk_entry_buffer_get_text(data->entryBuffer);
    int matricule = atoi(number) ;
    const char *region = gtk_combo_box_text_get_active_text(data->regionComboBox);

    // Print the entered number
    g_print("Entered Number: %d\n", matricule);
    g_print("Chosed option: %s\n", region);

    /** Modifier_R(fichier: Fd , Matricule: number , Region: region ); **/
    bool existe ;

    Chargement_index(&indexP);
    printfFichier("PERSONNEL-ANP_DZ.dat");
    printf("******************************************\n");
    Lecture = 0;
    Ecriture = 0;
    modifRegionMelitaire(matricule , region , &existe);

    if(existe == true){
        GtkWidget *warningDialog = gtk_message_dialog_new(GTK_WINDOW(data->mainWindow),
                                                          GTK_DIALOG_MODAL,
                                                          GTK_MESSAGE_WARNING,
                                                          GTK_BUTTONS_OK,
                                                          "Modification fait avec succès");   ///fait || faite
        gtk_dialog_run(GTK_DIALOG(warningDialog));
        gtk_widget_destroy(warningDialog);
    } else {
        GtkWidget *warningDialog = gtk_message_dialog_new(GTK_WINDOW(data->mainWindow),
                                                          GTK_DIALOG_MODAL,
                                                          GTK_MESSAGE_WARNING,
                                                          GTK_BUTTONS_OK,
                                                          "l'enregistrement que vous voulez changes sa region Milittaire n'existe pas");
        gtk_dialog_run(GTK_DIALOG(warningDialog));
        gtk_widget_destroy(warningDialog);
    }

    printf("le cout de modification est %d lecture et %d ecriture\n", Lecture, Ecriture);
    printfFichier("PERSONNEL-ANP_DZ.dat");
    printf("******************************************\n");

    // Clean up the allocated user data
    g_free(data);
}

// Function to create content for Button 7
void create_button7_content(GtkWidget *mainWindow, const char *buttonLabel) {

    // Create a new window for Button 1 content
    GtkWidget *buttonWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(buttonWindow), buttonLabel);
    gtk_window_set_default_size(GTK_WINDOW(buttonWindow), 300, 100);
    gtk_window_set_position(GTK_WINDOW(buttonWindow), GTK_WIN_POS_CENTER);

    // Create a box to hold the content in the new window
    GtkWidget *buttonBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(buttonWindow), buttonBox);

    /******************************* LE MATRICULE *******************************/
    // Create a label for Button 1 content
    GtkWidget *buttonLabelWidget = gtk_label_new("  Entrer le matricule de militaire que vous voulez modifier sa region  ");
    gtk_box_pack_start(GTK_BOX(buttonBox), buttonLabelWidget, FALSE, FALSE, 10);

    // Create an entry buffer for the number
    GtkEntryBuffer *entryBuffer = gtk_entry_buffer_new(NULL, 0);
    gtk_entry_buffer_set_max_length(entryBuffer, 6);  // Set maximum length to 6 characters

    // Create an entry widget for the number
    GtkWidget *entryNumber = gtk_entry_new_with_buffer(entryBuffer);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entryNumber), " Ex: 111111");

    g_signal_connect(entryNumber, "key-press-event", G_CALLBACK(on_entry_input), NULL);

    gtk_box_pack_start(GTK_BOX(buttonBox), entryNumber, FALSE, FALSE, 5);

    /******************************* LA NOUVELLE REGION *******************************/

    GtkWidget *buttonLabelWidget2 = gtk_label_new("   Entrer la nouvelle region militaire   ");
    gtk_box_pack_start(GTK_BOX(buttonBox), buttonLabelWidget2, FALSE, FALSE, 10);

    // Create a combo box for region selection
    GtkWidget *regionComboBox = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(regionComboBox), "1RM-Blida");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(regionComboBox), "2RM-Oran");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(regionComboBox), "3RM-Béchar");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(regionComboBox), "4RM-Ouargla");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(regionComboBox), "5RM-Constantine");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(regionComboBox), "6RM-Tamanrasset");

    gtk_box_pack_start(GTK_BOX(buttonBox), regionComboBox, FALSE, FALSE, 5);

    // Create a "Valider" button
    GtkWidget *validerButton = gtk_button_new_with_label("Modifier");

    // Create user data structure and set its values
    UserData2 *userData = g_new(UserData2, 1);
    userData->mainWindow = buttonWindow;
    userData->entryBuffer = entryBuffer;
    userData->regionComboBox = regionComboBox; // Pass the region combo box to UserData

    // Connect the callback function to the "clicked" signal of the "Valider" button
    g_signal_connect(validerButton, "clicked", G_CALLBACK(modifier_button_clicked), userData);

    // Pack widgets into the box
    gtk_box_pack_start(GTK_BOX(buttonBox), validerButton, FALSE, FALSE, 5);

    // Show all widgets in the new window
    gtk_widget_show_all(buttonWindow);

}

// Define a structure to hold both mainWindow and entryBuffer
typedef struct {
    GtkWidget *mainWindow;
    GtkWidget *searchEntry1;
    GtkWidget *searchEntry2;
} UserDataM1M2;

enum {
    COLUMN_TEXT,
    NUM_COLUMNS
};

// Function to create a new tree model row with the given data
void append_data_to_tree_store(GtkTreeStore *treestore, struct Tenrg *data) {
    GtkTreeIter iter;
    gtk_tree_store_append(treestore, &iter, NULL);

    gchar *text = g_strdup_printf(
            "  Nom & Prenom :   %s  %s\n"
            "  Né le :   %d/%d/%d  à  %s\n"
            "  Groupe sanguin :   %s\n"
            "  Région militaire :   %s\n"
            "  Grade :   %s\n"
            "  Force armée :   %s\n",
            data->Nom, data->Prenom,
            data->Date_Naissance.day, data->Date_Naissance.month, data->Date_Naissance.year,
            data->Wilaya_Naissance, data->Groupe_sanguin,
            data->Grade, data->Region_militaire, data->Force_armee
    );

    gtk_tree_store_set(treestore, &iter, COLUMN_TEXT, text, -1);

    g_free(text);
}

// Function to create a new tree view with the given data
GtkWidget *create_tree_view(struct Tenrg *data) {
    // Create a tree store with one string column
    GtkTreeStore *treestore = gtk_tree_store_new(NUM_COLUMNS, G_TYPE_STRING);

    // Append data to the tree store
    append_data_to_tree_store(treestore, data);

    // Create a tree view with the tree store as the model
    GtkWidget *treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(treestore));

    char str_matricule[8];  // Allocate enough space for the string
    sprintf(str_matricule, "  %d", data->Matricule);

    // Create a renderer for the text column
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes(str_matricule, renderer, "text", COLUMN_TEXT, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Apply the font to the tree view
    PangoFontDescription *font_desc = pango_font_description_from_string("Source Sans Pro 15");
    gtk_widget_override_font(treeview, font_desc);
    // Free the font description when done
    pango_font_description_free(font_desc);

    return treeview;
}

void searchM1M2_button_clicked(GtkWidget *widget, gpointer userdata){

    UserDataM1M2 *data = (UserDataM1M2 *)userdata;

    // Get the entered text from the search entry
    const char *searchText1 = gtk_entry_get_text(GTK_ENTRY(data->searchEntry1));
    const char *searchText2 = gtk_entry_get_text(GTK_ENTRY(data->searchEntry2));
    // Convert the entered text to an integer
    int M1 = atoi(searchText1);
    int M2 = atoi(searchText2);

    Chargement_index(&indexP);
    Lecture= 0;Ecriture = 0;

    int inf, sup;
    bool trouv1, trouv2;
    struct Tenrg e ;
    FILE *fichier;
    Ouvrire(&fichier, "PERSONNEL-ANP_DZ.dat", 'A');
    rechDicoTableIndex(M1, &trouv1, &inf, indexP);
    rechDicoTableIndex(M2, &trouv2, &sup, indexP);
    if (!trouv2) {
        --sup;
    }
    int i = inf;
    int j;
    Buffer *bufer = alloc_bloc(fichier);
    LireDir(fichier, indexP.tab[i].adress.nbBloc, bufer);

        GtkWidget *resultWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(resultWindow), "Result Window");
        gtk_window_set_default_size(GTK_WINDOW(resultWindow), 750, 650);
        gtk_window_set_position(GTK_WINDOW(resultWindow), GTK_WIN_POS_CENTER);

        // Create a ListBox to display the results
        GtkWidget *listbox = gtk_list_box_new();
        gtk_widget_set_name(listbox, "myListBox");

        // Add each row to the ListBox
        for (i = inf ; i <= sup ; i++) {
            if (indexP.tab[i].adress.nbBloc != indexP.tab[i - 1].adress.nbBloc) {
                free(bufer);
                bufer = alloc_bloc(fichier);
                LireDir(fichier, indexP.tab[i].adress.nbBloc, bufer);
            }

            j = indexP.tab[i].adress.nbEnrg; //j : l'indice de l'enregistrement
            // Recupere l'enrg qui a M1
            e = buf1.tab[j] ;
            printf("\n(pos_f_lindex=%d ; pos_f_Fd=%d) : '%d'",i,j,e.Matricule);

            GtkWidget *row = create_tree_view(&e);
            gtk_list_box_insert(GTK_LIST_BOX(listbox), row, -1);
        }

// Create a scrolled window
        GtkWidget *scrolledWindow = gtk_scrolled_window_new(NULL, NULL);

// Set the policy for the horizontal and vertical scrollbars
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

// Add the list box to the scrolled window
        gtk_container_add(GTK_CONTAINER(scrolledWindow), listbox);

// Set the scrolled window as the content of the main window
        gtk_container_add(GTK_CONTAINER(resultWindow), scrolledWindow);
        // Show all widgets in the new window
        gtk_widget_show_all(resultWindow);

    free(bufer);
    Fermer(fichier);
}

// Function to create content for Button 8 (List Box pour afficher les militaire)
void create_button8_content(GtkWidget *mainWindow, const char *buttonLabel) {

    // Create a new window for Button 1 content
    GtkWidget *buttonWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(buttonWindow), buttonLabel);
    gtk_window_set_default_size(GTK_WINDOW(buttonWindow), 300, 200);
    gtk_window_set_position(GTK_WINDOW(buttonWindow), GTK_WIN_POS_CENTER);

    // Create the vertical box to hold both M1 and M2 entries and the button
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    // Create the horizontal box for "Entrer M1" + EntryBuffer
    GtkWidget *hbox1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    // Create labels and entry buffers for M1
    GtkWidget *label1 = gtk_label_new("   Entrer M1 : ");
    GtkWidget *entry1 = gtk_entry_new();
    gtk_entry_set_max_length((GtkEntry *) entry1, 6);  // Set maximum length to 6 characters
    g_signal_connect(entry1, "key-press-event", G_CALLBACK(on_entry_input), NULL);

    // Pack M1 label and entry buffer into the first horizontal box
    gtk_box_pack_start(GTK_BOX(hbox1), label1, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox1), entry1, FALSE, FALSE, 5);

    // Create the horizontal box for "Entrer M2" + EntryBuffer
    GtkWidget *hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    // Create labels and entry buffers for M2
    GtkWidget *label2 = gtk_label_new("   Entrer M2 : ");
    GtkWidget *entry2 = gtk_entry_new();
    gtk_entry_set_max_length((GtkEntry *) entry2, 6);  // Set maximum length to 6 characters
    g_signal_connect(entry2, "key-press-event", G_CALLBACK(on_entry_input), NULL);

    // Pack M2 label and entry buffer into the second horizontal box
    gtk_box_pack_start(GTK_BOX(hbox2), label2, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox2), entry2, FALSE, FALSE, 5);

    // Create the "Valider" button
    GtkWidget *validerButton = gtk_button_new_with_label("Valider");

    // Create user data structure and set its values
    UserDataM1M2 *userdata = g_new(UserDataM1M2, 1);
    userdata->mainWindow = buttonWindow;
    userdata->searchEntry1 = entry1 ;
    userdata->searchEntry2 = entry2 ;

    g_signal_connect(validerButton, "clicked", G_CALLBACK(searchM1M2_button_clicked), userdata);

    // Pack the horizontal boxes and "Valider" button into the vertical box
    gtk_box_pack_start(GTK_BOX(vbox), hbox1, FALSE, FALSE, 15);
    gtk_box_pack_start(GTK_BOX(vbox), hbox2, FALSE, FALSE, 15);
    gtk_box_pack_start(GTK_BOX(vbox), validerButton, FALSE, FALSE, 0);

    // Pack the vertical box into the main window
    gtk_container_add(GTK_CONTAINER(buttonWindow), vbox);

    // Show all widgets in the new window
    gtk_widget_show_all(buttonWindow);

}

// Function to create content for Button 9 (List Box pour afficher les militaire)
void create_button9_content(GtkWidget *mainWindow, const char *buttonLabel) {

}

// Callback function for button clicks
void button_clicked(GtkWidget *widget, gpointer data) {
    // Get the top-level window
    GtkWidget *mainWindow = gtk_widget_get_toplevel(widget);

    // Check which button is clicked and create content accordingly
    const char *buttonLabel = (const char *)data;
    if (strcmp(buttonLabel, "Chargement initiale") == 0) {
        // Create content based on Button 1 with the variables
        create_button1_content(mainWindow, buttonLabel);
    } else if (strcmp(buttonLabel, "Sauvegarde de l'index") == 0) {
        create_button2_content(mainWindow, buttonLabel);
    } else if (strcmp(buttonLabel, "Chargement de l'index") == 0) {
        create_button3_content(mainWindow, buttonLabel);
    } else if (strcmp(buttonLabel, "Recherche d'un militaire par son matricule") == 0) {
        create_button4_content(mainWindow, buttonLabel);
    } else if (strcmp(buttonLabel, "Insertion d'un nouveau enregistrement") == 0) {
        create_button5_content(mainWindow, buttonLabel);
    }else if (strcmp(buttonLabel, "Suppression d'un enregistrement") == 0) {
        create_button6_content(mainWindow, buttonLabel);
    } else if (strcmp(buttonLabel, "Modifier la region melitaire d'un enregistrement") == 0) {
        create_button7_content(mainWindow, buttonLabel);
    } else if (strcmp(buttonLabel, "Afficher les enregistrements qui ils ont [M1 , M2]") == 0) {
        create_button8_content(mainWindow, buttonLabel);
    } else if (strcmp(buttonLabel, "fct suppplimentaire") == 0) {
        create_button9_content(mainWindow, buttonLabel);
    }
        g_print("Button %s clicked!\n", buttonLabel);
}

gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    // Load the background image
    GdkPixbuf *background = gdk_pixbuf_new_from_file_at_size("C:\\Users\\Admin\\OneDrive\\Bureau\\TP\\TP_interface\\images\\image20.jpg", 1024, 1024, NULL); //6,7,12,13,17,19,20,21

    if (background != NULL) {
        // Get the actual size of the drawing area
        int width = gtk_widget_get_allocated_width(widget);
        int height = gtk_widget_get_allocated_height(widget);

        //printf("\n size %d * %d ", width, height);

        // Scale the background image proportionally to fit within the widget
        GdkPixbuf *scaled_background = gdk_pixbuf_scale_simple(
                background,
                width,
                height,
                GDK_INTERP_BILINEAR
        );

        // Center the scaled image within the widget
        int x_offset = (width - gdk_pixbuf_get_width(scaled_background)) / 2;
        int y_offset = (height - gdk_pixbuf_get_height(scaled_background)) / 2;

        // Use the scaled and centered image for drawing
        gdk_cairo_set_source_pixbuf(cr, scaled_background, x_offset, y_offset);
        cairo_paint(cr);

        // Clean up
        g_object_unref(scaled_background);
        g_object_unref(background);

    } else {
        g_printerr("Error loading the background image\n");
    }

    // Let the default handler do its job
    return FALSE;
}

gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    // Get the size of the drawing area
    int width, height;
    gtk_widget_get_size_request(widget, &width, &height);

    // Create a linear gradient from top to bottom
    cairo_pattern_t *gradient = cairo_pattern_create_linear(1.0, 1.0, 1.0, height);
    cairo_pattern_add_color_stop_rgb(gradient, 0.0, 0.0,0.5, 0.5);  // Start color (blue)
    cairo_pattern_add_color_stop_rgb(gradient, 1.0, 1.0, 1.0, 1.0);  // End color (white)

    // Set the source pattern and paint
    cairo_set_source(cr, gradient);
    cairo_paint(cr);

    // Clean up the gradient
    cairo_pattern_destroy(gradient);

    // Let the default handler do its job
    return FALSE;
}

GtkWidget *splash_window;
GtkWidget *window;

static gboolean splash_timeout(gpointer data) {
    gtk_widget_hide(splash_window);
    gtk_widget_show_all(window); // Show the main window after hiding the splash screen
    return G_SOURCE_REMOVE;
}

void on_image_clicked(GtkWidget *widget, gpointer data) {
    // Now, quit the GTK main loop
    gtk_main_quit();
}


int main(int argc, char *argv[]) {

    /*********************************** BACK END *********************************/
///    Ouvrire(&f,"Matricule_Index.idx",'A');
//        Ouvrire(&Fi, "PERSONNEL-ANP_DZ.dat", 'N');
//        chargementInitial(Fi,1200);
//        printfFichier("PERSONNEL-ANP_DZ.dat");
//        Chargement_index(&index1);
//        rechDicoTableIndex(997885,&trouv,&k,index1);
//    Chargement_indexS(&indexS1, 1);
//    Chargement_indexS(&indexS2, 2);
//        printIndexPrimaire("Matricule_Index.idx");
//        printf("%d dans %d-%d a %d",index1.tab[k].cle,index1.tab[k].adress.nbBloc,index1.tab[k].adress.nbEnrg,k);
//        suppersionEnrg(997885, &indexS1, &indexS2);

//    suppressionForceArme("Armee_de_terre", &indexS1, &indexS2);
//    afficherfichierindex("index.dat(force_militaire)");
//    printf("-------------------------------------------------------------------------\n");
//    afficherfichierindex("index(region_militaire).dat");
//    Sauvegarde_IndexS(indexS1,1);
//    Sauvegarde_IndexS(indexS2,2);

    //Fermer(Fi);

    //initIndex(&index1);

    /*****************************************************************************/
    // Initialize GTK
    gtk_init(&argc, &argv);

    splash_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

// Set window properties
    gtk_window_set_title(GTK_WINDOW(splash_window), "Your Program Name");
    gtk_window_set_decorated(GTK_WINDOW(splash_window), FALSE); // Remove title bar and borders
    gtk_window_set_position(GTK_WINDOW(splash_window), GTK_WIN_POS_CENTER); // Center the window
    gtk_widget_set_size_request(splash_window, 600, 350); // Set window size

    // Set the draw signal handler for the main window
    g_signal_connect(splash_window, "draw", G_CALLBACK(on_draw), NULL);

    // Allow the window to be painted
    gtk_widget_set_app_paintable(splash_window, TRUE);

    // Create a vertical box to hold the title label and buttons
    GtkWidget *vvbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_container_add(GTK_CONTAINER(splash_window), vvbox);

    // Create the title label
    GtkWidget *titleLabel1 = gtk_label_new("ECOLE NATIONALE SUPERIEURE DE L'INFORMATIQUE");
    PangoFontDescription *font_desc1 = pango_font_description_new();
    pango_font_description_set_size(font_desc1, PANGO_SCALE * 14); // Set font size
    pango_font_description_set_weight(font_desc1, PANGO_WEIGHT_SEMIBOLD);
    gtk_widget_override_font(titleLabel1, font_desc1);
    gtk_widget_set_halign(titleLabel1, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(vvbox), titleLabel1, FALSE, FALSE, 10);

    // Create the title label2
    GtkWidget *titleLabel2 = gtk_label_new("TP2 : GestMilitaireANP");
    PangoFontDescription *font_desc2 = pango_font_description_new();
    pango_font_description_set_size(font_desc2, PANGO_SCALE * 25); // Set font size
    pango_font_description_set_weight(font_desc2, PANGO_WEIGHT_BOLD);
    pango_font_description_set_family(font_desc2, "Monospace"); // Set font family
    gtk_widget_override_font(titleLabel2, font_desc2);
    gtk_widget_set_halign(titleLabel2, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(vvbox), titleLabel2, FALSE, FALSE, 75);
    // Set the background color to white
    GdkRGBA white;
    gdk_rgba_parse(&white, "white");
    gtk_widget_override_color(titleLabel2, GTK_STATE_FLAG_NORMAL, &white);

//    GtkWidget *label = gtk_label_new("Réaliser par : Lyes & Yasser");
//    // Create a PangoFontDescription for the desired font
//    PangoFontDescription *font_desc0 = pango_font_description_new();
//    gtk_widget_set_halign(label, GTK_ALIGN_START);
//    pango_font_description_set_family(font_desc0, "Arial"); // Set font family
//    pango_font_description_set_size(font_desc0, PANGO_SCALE * 12); // Set font size
//    pango_font_description_set_weight(font_desc2, PANGO_WEIGHT_THIN);
//    // Apply the font to the label
//    gtk_widget_override_font(label, font_desc0);
//    // Clean up the font description
//    pango_font_description_free(font_desc0);
//    gtk_box_pack_start(GTK_BOX(vvbox), label, FALSE, FALSE, 15);

    // Create the title label3
    GtkWidget *titleLabel3 = gtk_label_new("Copyright @ tout droits resérvés");
    PangoFontDescription *font_desc3 = pango_font_description_new();
    pango_font_description_set_weight(font_desc3, PANGO_WEIGHT_THIN);
    gtk_widget_override_font(titleLabel3, font_desc3);
    gtk_widget_set_halign(titleLabel3, GTK_ALIGN_CENTER);
    pango_font_description_set_size(font_desc3, PANGO_SCALE * 8); // Set font size
    gtk_box_pack_start(GTK_BOX(vvbox), titleLabel3, FALSE, FALSE, 10);

// Pack the label box into the splash window
    gtk_container_add(GTK_CONTAINER(splash_window), vvbox);
/************************************************************************************/

    // Create the main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "LYES");
    gtk_window_set_default_size(GTK_WINDOW(window), 750, 700);
    // Make it full screen : gtk_window_fullscreen(GTK_WINDOW(window));
    //gtk_window_fullscreen(GTK_WINDOW(window));
    gtk_container_set_border_width(GTK_CONTAINER(window), 25);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    // Set the draw signal handler for the main window
    g_signal_connect(window, "draw", G_CALLBACK(on_draw_event), NULL);

    // Allow the window to be painted
    gtk_widget_set_app_paintable(window, TRUE);

    // Create a vertical box to hold the title label and buttons
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 25);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *image = gtk_image_new_from_file("C:\\Users\\Admin\\OneDrive\\Bureau\\TP\\TP_interface\\images\\exit.png");
    gtk_widget_set_size_request(image, 25, 25);
// Create a button that holds the image
    GtkWidget *button_img = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(button_img), image);
    g_signal_connect(button_img, "clicked", G_CALLBACK(on_image_clicked), NULL);

    // Set button relief style to GTK_RELIEF_NONE
    //gtk_button_set_relief(GTK_BUTTON(button_img), GTK_RELIEF_NONE);

    // Set background color to transparent
    GdkRGBA transparent;
    transparent.red = 0.0;
    transparent.green = 0.0;
    transparent.blue = 0.0;
    transparent.alpha = 0.0;

    gtk_widget_override_background_color(button_img, GTK_STATE_FLAG_NORMAL, &transparent);


// Create a horizontal box to hold the image
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 425);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

// Pack the image into the horizontal box with left alignment
    gtk_box_pack_start(GTK_BOX(hbox), button_img, FALSE, FALSE, 0);
    gtk_widget_set_halign(button_img, GTK_ALIGN_START);

// Create the title label
    GtkWidget *titleLabel = gtk_label_new("BIENVENUE DANS NOTRE PROGRAMME");
    PangoFontDescription *font_desc = pango_font_description_from_string("Britannic 17");

    pango_font_description_set_weight(font_desc, PANGO_WEIGHT_HEAVY);
    gtk_widget_override_font(titleLabel, font_desc);

// Create a box to hold the title label and center it
    GtkWidget *labelBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(labelBox), titleLabel, FALSE, FALSE, 5);
    gtk_widget_set_halign(labelBox, GTK_ALIGN_CENTER);

// Pack the labelBox into the horizontal box
    gtk_box_pack_start(GTK_BOX(hbox), labelBox, FALSE, FALSE, 5);

// Don't forget to free the font description when you're done
    pango_font_description_free(font_desc);

    // Create the second label
    GtkWidget *secondLabel = gtk_label_new("</>   M E N U   </>");
    PangoFontDescription *font_desc00 = pango_font_description_from_string("Berlin Sans FB 15"); //* Britannic 17 *//

    pango_font_description_set_weight(font_desc00, PANGO_WEIGHT_MEDIUM);
    gtk_widget_override_font(secondLabel, font_desc00);
    gtk_widget_set_halign(secondLabel, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox), secondLabel, FALSE, FALSE, 15);
    // Don't forget to free the font description when you're done
    pango_font_description_free(font_desc00);

    // Create a box to hold the buttons and center it
    GtkWidget *buttonBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_halign(buttonBox, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(vbox), buttonBox);

    // Create five buttons with the color orange
    const char *button_labels[] = {"Chargement initiale", "Sauvegarde de l'index", "Chargement de l'index", "Recherche d'un militaire par son matricule", "Insertion d'un nouveau enregistrement", "Suppression d'un enregistrement", "Modifier la region melitaire d'un enregistrement", "Afficher les enregistrements qui ils ont [M1 , M2]", "fct suppplimentaire"};

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    //const gchar *css = "button#custom-button:hover { background-color: #e3750e; color: #570861; }\n";  // Adjust hover color as needed
    //gtk_css_provider_load_from_data(provider, css, -1, NULL);

    gtk_css_provider_load_from_data(provider, "button#custom-button:hover { color: #007bff; }", -1, NULL);

    for (int i = 0; i < 9; ++i) {

        GtkWidget *button = gtk_button_new_with_label(button_labels[i]);
        gtk_widget_set_name(button, "custom-button");  // Give a name to each button

        // Create a PangoFontDescription for the desired font
        PangoFontDescription *fontDesc_menu = pango_font_description_from_string("Source Sans Pro 14");  // Adjust font name and size as needed
        gtk_widget_override_font(gtk_bin_get_child(GTK_BIN(button)), fontDesc_menu);
        // Free the font description when finished
        pango_font_description_free(fontDesc_menu);

        gtk_box_pack_start(GTK_BOX(buttonBox), button, FALSE, FALSE, 5);
        gtk_widget_set_size_request(button, 400, 30);

        // Set border-radius for rounded corners
        GtkCssProvider *provider_btn = gtk_css_provider_new();
        const gchar *cssData = "button { border: 1px solid #ffffff; border-radius: 19px; }";
        gtk_css_provider_load_from_data(provider_btn, cssData, -1, NULL);
        gtk_style_context_add_provider(gtk_widget_get_style_context(button), GTK_STYLE_PROVIDER(provider_btn), GTK_STYLE_PROVIDER_PRIORITY_USER);
        g_object_unref(provider_btn);

        // Connect the callback function to the "clicked" signal of each button
        g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), (gpointer)button_labels[i]);
    }

    // Show all widgets
    gtk_widget_show_all(window);
    gtk_widget_hide(window);

    // Start the splash timeout function
    g_timeout_add_seconds(2, splash_timeout, NULL);

    // Show the splash window and run the main loop
    gtk_widget_show_all(splash_window);

    // Connect the "destroy" signal of the window to gtk_main_quit
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Run the GTK main loop
    gtk_main();

    return 0;
}