#include <stdio.h>
#include <gtk/gtk.h> //for the gui

//every button for every motor/function of the robotic arm

//for this with gtk, widgets are used for buttons

//for turning continueously while holding button(motor movement), use on pressed and on released

//light (on, off)
static void on_light_on_button_clicked(GtkWidget *widget, gpointer data)
{
    //call to device
    printf("Debugging: light on\n");
}
static void on_light_off_button_clicked(GtkWidget *widget, gpointer data)
{
    //call to device
    printf("Debugging: light off\n");
}



//body (clockwise = +, anticlockwise = -)
static void on_body_pos_button_pressed(GtkWidget *widget, gpointer data)
{
    //call to device
    printf("Debugging: body turning clockwise\n");
}
//ask petr if this method of controlling will work with the driver
static void on_body_pos_button_released(GtkWidget *widget, gpointer data)
{
    //call to device
    printf("Debugging: body turning clockwise stopped\n");
}

static void on_body_neg_button_pressed(GtkWidget *widget, gpointer data)
{
    //call to device
    printf("Debugging: body turninga anticlockwise\n");
}

static void on_body_neg_button_released(GtkWidget *widget, gpointer data)
{
    //call to device
    printf("Debugging: body turning anticlockwise stopped\n");
}


//shoulder (clockwise = +, anticlockwise = -)
static void on_shoulder_pos_button_pressed(GtkWidget *widget, gpointer data)
{
    //call to device
    printf("Debugging: shoulder turning clockwise\n");
}

static void on_shoulder_pos_button_released(GtkWidget *widget, gpointer data)
{
    //call to device
    printf("Debugging: shoulder turning clockwise stopped\n");
}

static void on_shoulder_neg_button_pressed(GtkWidget *widget, gpointer data)
{
    //call to device
    printf("Debugging: shoulder turning anticlockwise\n");
}

static void on_shoulder_neg_button_released(GtkWidget *widget, gpointer data)
{
    //call to device
    printf("Debugging: shoulder turning anticlockwise stopped\n");
}

//cp shoudler but change for elbow, wrist, claw 

//elbow (clockwise = +, anticlockwise = -)
static void on_elbow_pos_button_pressed(GtkWidget *widget, gpointer data)
{
    //call to device
    printf("Debugging: elbow turning clockwise\n");
}

static void on_elbow_pos_button_released(GtkWidget *widget, gpointer data)
{
    //call to device
    printf("Debugging: elbow turning clockwise stopped\n");
}

static void on_elbow_neg_button_pressed(GtkWidget *widget, gpointer data)
{
    //call to device
    printf("Debugging: elbow turning anticlockwise\n");
}

static void on_elbow_neg_button_released(GtkWidget *widget, gpointer data)
{
    //call to device
    printf("Debugging: elbow turning anticlockwise stopped\n");
}



//wrist (clockwise = +, anticlockwise = -)
static void on_wrist_pos_button_pressed(GtkWidget *widget, gpointer data)
{
    //call to device
    printf("Debugging: wrist turning clockwise\n");
}

static void on_wrist_pos_button_released(GtkWidget *widget, gpointer data)
{
    //call to device
    printf("Debugging: wrist turning clockwise stopped\n");
}

static void on_wrist_neg_button_pressed(GtkWidget *widget, gpointer data)
{
    //call to device
    printf("Debugging: wrist turning anticlockwise\n");
}

static void on_wrist_neg_button_released(GtkWidget *widget, gpointer data)
{
    //call to device
    printf("Debugging: wrist turning anticlockwise stopped\n");
}



//claw (open = +, close = -)
static void on_claw_pos_button_pressed(GtkWidget *widget, gpointer data)
{
    //call to device
    printf("Debugging: claw opening\n");
}

static void on_claw_pos_button_released(GtkWidget *widget, gpointer data)
{
    //call to device
    printf("Debugging: claw opening stopped\n");
}

static void on_claw_neg_button_pressed(GtkWidget *widget, gpointer data)
{
    //call to device
    printf("Debugging: claw closing\n");
}

static void on_claw_neg_button_released(GtkWidget *widget, gpointer data)
{
    //call to device
    printf("Debugging: claw closing stopped\n");
}

//key press event callback
static gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    switch(event->keyval)
    {
	case GDK_KEY_1:
            on_light_on_button_clicked(widget, data);
            break;
        case GDK_KEY_2:
            on_light_off_button_clicked(widget, data);
            break;
        case GDK_KEY_k:
            on_body_pos_button_pressed(widget, data);
            break;
        case GDK_KEY_o:
            on_body_neg_button_pressed(widget, data);
            break;
        case GDK_KEY_j:
            on_shoulder_pos_button_pressed(widget, data);
            break;
        case GDK_KEY_i:
            on_shoulder_neg_button_pressed(widget, data);
            break;
        case GDK_KEY_f:
            on_elbow_pos_button_pressed(widget, data);
            break;
        case GDK_KEY_r:
            on_elbow_neg_button_pressed(widget, data);
            break;
        case GDK_KEY_d:
            on_wrist_pos_button_pressed(widget, data);
            break;
        case GDK_KEY_e:
            on_wrist_neg_button_pressed(widget, data);
            break;
        case GDK_KEY_s:
            on_claw_pos_button_pressed(widget, data);
            break;
        case GDK_KEY_w:
            on_claw_neg_button_pressed(widget, data);
            break;
    }
    return FALSE;
}

//key release event callback
static gboolean on_key_release(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    switch(event->keyval)
    {
        case GDK_KEY_k:
            on_body_pos_button_released(widget, data);
            break;
        case GDK_KEY_o:
            on_body_neg_button_released(widget, data);
            break;
        case GDK_KEY_j:
            on_shoulder_pos_button_released(widget, data);
            break;
        case GDK_KEY_i:
            on_shoulder_neg_button_released(widget, data);
            break;
        case GDK_KEY_f:
            on_elbow_pos_button_released(widget, data);
            break;
        case GDK_KEY_r:
            on_elbow_neg_button_released(widget, data);
            break;
        case GDK_KEY_d:
            on_wrist_pos_button_released(widget, data);
            break;
        case GDK_KEY_e:
            on_wrist_neg_button_released(widget, data);
            break;
        case GDK_KEY_s:
            on_claw_pos_button_released(widget, data);
            break;
        case GDK_KEY_w:
            on_claw_neg_button_released(widget, data);
            break;
    }
    return FALSE;
}


int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv); //initialising gtk - the gui lib I'm using

    //gui window setup
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //instantiating a window called window (so imaginative I know), TOPLEVEL allows window title etc
    gtk_window_set_title(GTK_WINDOW(window), "Robotic Arm Controller"); //window title
    gtk_container_set_border_width(GTK_CONTAINER(window), 20); //window border width size

    //how do i terminate the process when i close the window? signalssssss
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL); //closing window terminates the program (good). window = the widget sending the signal, "destroy" = the name of the signal, G_CALLBACK(gtk_main_quiit) = ran when signal sent, NULL = space for returning user data (NA)

    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), NULL);
    g_signal_connect(window, "key-release-event", G_CALLBACK(on_key_release), NULL);
    gtk_widget_set_can_focus(window, TRUE);

    //for container: box pack, with a vbox and a hbox. (vertical an dhorizontal organisation for labels and buttons)
    //for each label in the vbox (down the way), each will have a hbox for the two option buttons)

    //vbox for everything
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); //5 pixel spacing
    gtk_container_add(GTK_CONTAINER(window), vbox);//since container_add can only add one element, the one element we add is our main vbox


    //light label
    GtkWidget *light_label = gtk_label_new("Light");
    gtk_box_pack_start(GTK_BOX(vbox), light_label, FALSE, FALSE, 0);

    //hbox for light buttons
    GtkWidget *light_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); //5 pixel spacing
    gtk_box_pack_start(GTK_BOX(vbox), light_hbox, FALSE, FALSE, 0);
    //light on button
    GtkWidget *light_on_button = gtk_button_new_with_label("On (1)");
    g_signal_connect(light_on_button, "clicked", G_CALLBACK(on_light_on_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(light_hbox), light_on_button, TRUE, TRUE, 0);
    //light off button
    GtkWidget *light_off_button = gtk_button_new_with_label("Off (2)");
    g_signal_connect(light_off_button, "clicked", G_CALLBACK(on_light_off_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(light_hbox), light_off_button, TRUE, TRUE, 0);


    //body label
    GtkWidget *body_label = gtk_label_new("Body");
    gtk_box_pack_start(GTK_BOX(vbox), body_label, FALSE, FALSE, 0);

    //hbox for body buttons
    GtkWidget *body_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); //5 pixel spacing
    gtk_box_pack_start(GTK_BOX(vbox), body_hbox, FALSE, FALSE, 0);
    //body pos button (pressed and released for continuous holding button)
    GtkWidget *body_pos_button = gtk_button_new_with_label("+ (k)");
    g_signal_connect(body_pos_button, "pressed", G_CALLBACK(on_body_pos_button_pressed), NULL);
    g_signal_connect(body_pos_button, "released", G_CALLBACK(on_body_pos_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(body_hbox), body_pos_button, TRUE, TRUE, 0);
    //body neg button
    GtkWidget *body_neg_button = gtk_button_new_with_label("- (o)");
    g_signal_connect(body_neg_button, "pressed", G_CALLBACK(on_body_neg_button_pressed), NULL);
    g_signal_connect(body_neg_button, "released", G_CALLBACK(on_body_neg_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(body_hbox), body_neg_button, TRUE, TRUE, 0);


    //shoulder label
    GtkWidget *shoulder_label = gtk_label_new("Shoulder");
    gtk_box_pack_start(GTK_BOX(vbox), shoulder_label, FALSE, FALSE, 0);

    //hbox for shoulder buttons
    GtkWidget *shoulder_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); //5 pixel spacing
    gtk_box_pack_start(GTK_BOX(vbox), shoulder_hbox, FALSE, FALSE, 0);
    //shoulder pos button (pressed and released for continuous holding button)
    GtkWidget *shoulder_pos_button = gtk_button_new_with_label("+ (j)");
    g_signal_connect(shoulder_pos_button, "pressed", G_CALLBACK(on_shoulder_pos_button_pressed), NULL);
    g_signal_connect(shoulder_pos_button, "released", G_CALLBACK(on_shoulder_pos_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(shoulder_hbox), shoulder_pos_button, TRUE, TRUE, 0);
    //shoulder neg button
    GtkWidget *shoulder_neg_button = gtk_button_new_with_label("- (i)");
    g_signal_connect(shoulder_neg_button, "pressed", G_CALLBACK(on_shoulder_neg_button_pressed), NULL);
    g_signal_connect(shoulder_neg_button, "released", G_CALLBACK(on_shoulder_neg_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(shoulder_hbox), shoulder_neg_button, TRUE, TRUE, 0);


    //elbow label
    GtkWidget *elbow_label = gtk_label_new("Elbow");
    gtk_box_pack_start(GTK_BOX(vbox), elbow_label, FALSE, FALSE, 0);

    //hbox for elbow buttons
    GtkWidget *elbow_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); //5 pixel spacing
    gtk_box_pack_start(GTK_BOX(vbox), elbow_hbox, FALSE, FALSE, 0);
    //elbow pos button (pressed and released for continuous holding button)
    GtkWidget *elbow_pos_button = gtk_button_new_with_label("+ (f)");
    g_signal_connect(elbow_pos_button, "pressed", G_CALLBACK(on_elbow_pos_button_pressed), NULL);
    g_signal_connect(elbow_pos_button, "released", G_CALLBACK(on_elbow_pos_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(elbow_hbox), elbow_pos_button, TRUE, TRUE, 0);
    //elbow neg button
    GtkWidget *elbow_neg_button = gtk_button_new_with_label("- (r)");
    g_signal_connect(elbow_neg_button, "pressed", G_CALLBACK(on_elbow_neg_button_pressed), NULL);
    g_signal_connect(elbow_neg_button, "released", G_CALLBACK(on_elbow_neg_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(elbow_hbox), elbow_neg_button, TRUE, TRUE, 0);



    //wrist label
    GtkWidget *wrist_label = gtk_label_new("Wrist");
    gtk_box_pack_start(GTK_BOX(vbox), wrist_label, FALSE, FALSE, 0);

    //hbox for wrist buttons
    GtkWidget *wrist_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); //5 pixel spacing
    gtk_box_pack_start(GTK_BOX(vbox), wrist_hbox, FALSE, FALSE, 0);
    //wrist pos button (pressed and released for continuous holding button)
    GtkWidget *wrist_pos_button = gtk_button_new_with_label("+ (d)");
    g_signal_connect(wrist_pos_button, "pressed", G_CALLBACK(on_wrist_pos_button_pressed), NULL);
    g_signal_connect(wrist_pos_button, "released", G_CALLBACK(on_wrist_pos_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(wrist_hbox), wrist_pos_button, TRUE, TRUE, 0);
    //wrist neg button
    GtkWidget *wrist_neg_button = gtk_button_new_with_label("- (e)");
    g_signal_connect(wrist_neg_button, "pressed", G_CALLBACK(on_wrist_neg_button_pressed), NULL);
    g_signal_connect(wrist_neg_button, "released", G_CALLBACK(on_wrist_neg_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(wrist_hbox), wrist_neg_button, TRUE, TRUE, 0);



    //claw label
    GtkWidget *claw_label = gtk_label_new("Claw");
    gtk_box_pack_start(GTK_BOX(vbox), claw_label, FALSE, FALSE, 0);

    //hbox for claw buttons
    GtkWidget *claw_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); //5 pixel spacing
    gtk_box_pack_start(GTK_BOX(vbox), claw_hbox, FALSE, FALSE, 0);
    //claw pos button (pressed and released for continuous holding button)
    GtkWidget *claw_pos_button = gtk_button_new_with_label("+ (s)");
    g_signal_connect(claw_pos_button, "pressed", G_CALLBACK(on_claw_pos_button_pressed), NULL);
    g_signal_connect(claw_pos_button, "released", G_CALLBACK(on_claw_pos_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(claw_hbox), claw_pos_button, TRUE, TRUE, 0);
    //claw neg button
    GtkWidget *claw_neg_button = gtk_button_new_with_label("- (w)");
    g_signal_connect(claw_neg_button, "pressed", G_CALLBACK(on_claw_neg_button_pressed), NULL);
    g_signal_connect(claw_neg_button, "released", G_CALLBACK(on_claw_neg_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(claw_hbox), claw_neg_button, TRUE, TRUE, 0);

    //labels and buttons done
    gtk_widget_show_all(window);

    gtk_main(); //looping
    return 0;
}