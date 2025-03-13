#include <stdio.h>
#include <gtk/gtk.h> //for the gui
#include <fcntl.h>
#include <unistd.h>

#define DEVICE_PATH "/dev/A37JN_Robot_arm"

#include <stdlib.h>
#include <linux/joystick.h>

#define JOYSTICK_DEV "/dev/input/js0" // joystick driver
#define AXIS_THRESHOLD 1000  

//booleans to keep track of when a key is pressed (to prevent repeated calling)
static gboolean key_light_on = FALSE;
static gboolean key_light_off = FALSE;
static gboolean key_base_pos = FALSE;
static gboolean key_base_neg = FALSE;
static gboolean key_shoulder_pos = FALSE;
static gboolean key_shoulder_neg = FALSE;
static gboolean key_elbow_pos = FALSE;
static gboolean key_elbow_neg = FALSE;
static gboolean key_wrist_pos = FALSE;
static gboolean key_wrist_neg = FALSE;
static gboolean key_claw_pos = FALSE;
static gboolean key_claw_neg = FALSE;

//every button for every motor/function of the robotic arm
//for this with gtk, widgets are used for buttons
//for turning continuously while holding button(motor movement), use on pressed and on released

/**
 * Sends a command to the A37JN robot arm via the device file.
 * @param command: The command string to send (e.g., "base:left\n").
 * @return 0 on success, -1 on failure.
 */

int send_robot_command(const char *command) {
    const int fd = open(DEVICE_PATH, O_WRONLY);  // Open device file for writing
    if (fd == -1) {
        perror("Error opening device file");
        return -1;
    }

    const ssize_t bytes_written = write(fd, command, strlen(command));
    if (bytes_written == -1) {
        perror("Error writing to device file");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}


//light (on, off)
static void on_light_on_button_clicked(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("led:on");
    printf("Debugging: light on\n");
}

static void on_light_off_button_clicked(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("led:off");
    printf("Debugging: light off\n");
}

//base (clockwise = +, anticlockwise = -)
static void on_base_pos_button_pressed(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("base:left");
    printf("Debugging: base turning clockwise\n");
}

//ask petr if this method of controlling will work with the driver
static void on_base_pos_button_released(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("base:stop");
    printf("Debugging: base turning clockwise stopped\n");
}

static void on_base_neg_button_pressed(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("base:right");
    printf("Debugging: base turning anticlockwise\n");
}

static void on_base_neg_button_released(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("base:stop");
    printf("Debugging: base turning anticlockwise stopped\n");
}

//shoulder (clockwise = +, anticlockwise = -)
static void on_shoulder_pos_button_pressed(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("shoulder:down");
    printf("Debugging: shoulder turning clockwise\n");
}

static void on_shoulder_pos_button_released(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("shoulder:stop");
    printf("Debugging: shoulder turning clockwise stopped\n");
}

static void on_shoulder_neg_button_pressed(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("shoulder:up");
    printf("Debugging: shoulder turning anticlockwise\n");
}

static void on_shoulder_neg_button_released(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("shoulder:stop");
    printf("Debugging: shoulder turning anticlockwise stopped\n");
}

//elbow (clockwise = +, anticlockwise = -)
static void on_elbow_pos_button_pressed(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("elbow:down");
    printf("Debugging: elbow turning clockwise\n");
}

static void on_elbow_pos_button_released(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("elbow:stop");
    printf("Debugging: elbow turning clockwise stopped\n");
}

static void on_elbow_neg_button_pressed(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("elbow:up");
    printf("Debugging: elbow turning anticlockwise\n");
}

static void on_elbow_neg_button_released(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("elbow:stop");
    printf("Debugging: elbow turning anticlockwise stopped\n");
}

//wrist (clockwise = +, anticlockwise = -)
static void on_wrist_pos_button_pressed(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("wrist:down");
    printf("Debugging: wrist turning clockwise\n");
}

static void on_wrist_pos_button_released(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("wrist:stop");
    printf("Debugging: wrist turning clockwise stopped\n");
}

static void on_wrist_neg_button_pressed(GtkWidget *widget, gpointer data){
    //call to device
    send_robot_command("wrist:up");
    printf("Debugging: wrist turning anticlockwise\n");
}

static void on_wrist_neg_button_released(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("wrist:stop");
    printf("Debugging: wrist turning anticlockwise stopped\n");
}

//claw (open = +, close = -)
static void on_claw_pos_button_pressed(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("claw:open");
    printf("Debugging: claw opening\n");
}

static void on_claw_pos_button_released(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("claw:stop");
    printf("Debugging: claw opening stopped\n");
}

static void on_claw_neg_button_pressed(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("claw:close");
    printf("Debugging: claw closing\n");
}

static void on_claw_neg_button_released(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("claw:stop");
    printf("Debugging: claw closing stopped\n");
}

// JOYSTICK INPUTS
if (js.type == JS_EVENT_BUTTON && js.value == 1) {
    switch (js.number) {
        case 6:
            send_robot_command("led:on"); // Turns on LED
            break;
        case 7:
            send_robot_command("led:off"); // Turns on LED
            break;
        // case 0:
        //     printf("Trigger Activated\n");
        //     break;
        // case 1:
        //     printf("Thumb Button Triggered\n");
        //     break;
        // case 2:
        //     printf("Top Button 1\n");
        //     break;
        // case 3:
        //     printf("Top Button 2\n");
        //     break;
        // default:
        //     printf("Button %d pressed\n", js.number);
        //     break;
    }
}

//key press event callback
static gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    switch(event->keyval)
    {
        case GDK_KEY_1:
            if(!key_light_on)
            {
                key_light_on = TRUE;
                on_light_on_button_clicked(widget, data);
            }
            break;
        case GDK_KEY_2:
            if(!key_light_off)
            {
                key_light_off = TRUE;
                on_light_off_button_clicked(widget, data);
            }
            break;
        case GDK_KEY_k:
            if(!key_base_pos)
            {
                key_base_pos = TRUE;
                on_base_pos_button_pressed(widget, data);
            }
            break;
        case GDK_KEY_o:
            if(!key_base_neg)
            {
                key_base_neg = TRUE;
                on_base_neg_button_pressed(widget, data);
            }
            break;
        case GDK_KEY_j:
            if(!key_shoulder_pos)
            {
                key_shoulder_pos = TRUE;
                on_shoulder_pos_button_pressed(widget, data);
            }
            break;
        case GDK_KEY_i:
            if(!key_shoulder_neg)
            {
                key_shoulder_neg = TRUE;
                on_shoulder_neg_button_pressed(widget, data);
            }
            break;
        case GDK_KEY_f:
            if(!key_elbow_pos)
            {
                key_elbow_pos = TRUE;
                on_elbow_pos_button_pressed(widget, data);
            }
            break;
        case GDK_KEY_r:
            if(!key_elbow_neg)
            {
                key_elbow_neg = TRUE;
                on_elbow_neg_button_pressed(widget, data);
            }
            break;
        case GDK_KEY_d:
            if(!key_wrist_pos)
            {
                key_wrist_pos = TRUE;
                on_wrist_pos_button_pressed(widget, data);
            }
            break;
        case GDK_KEY_e:
            if(!key_wrist_neg)
            {
                key_wrist_neg = TRUE;
                on_wrist_neg_button_pressed(widget, data);
            }
            break;
        case GDK_KEY_s:
            if(!key_claw_pos)
            {
                key_claw_pos = TRUE;
                on_claw_pos_button_pressed(widget, data);
            }
            break;
        case GDK_KEY_w:
            if(!key_claw_neg)
            {
                key_claw_neg = TRUE;
                on_claw_neg_button_pressed(widget, data);
            }
            break;
    }
    return FALSE;
}

//key release event callback
static gboolean on_key_release(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    switch(event->keyval)
    {
        case GDK_KEY_1:
            key_light_on = FALSE;
            break;
        case GDK_KEY_2:
            key_light_off = FALSE;
            break;
        case GDK_KEY_k:
            if(key_base_pos)
            {
                on_base_pos_button_released(widget, data);
                key_base_pos = FALSE;
            }
            break;
        case GDK_KEY_o:
            if(key_base_neg)
            {
                on_base_neg_button_released(widget, data);
                key_base_neg = FALSE;
            }
            break;
        case GDK_KEY_j:
            if(key_shoulder_pos)
            {
                on_shoulder_pos_button_released(widget, data);
                key_shoulder_pos = FALSE;
            }
            break;
        case GDK_KEY_i:
            if(key_shoulder_neg)
            {
                on_shoulder_neg_button_released(widget, data);
                key_shoulder_neg = FALSE;
            }
            break;
        case GDK_KEY_f:
            if(key_elbow_pos)
            {
                on_elbow_pos_button_released(widget, data);
                key_elbow_pos = FALSE;
            }
            break;
        case GDK_KEY_r:
            if(key_elbow_neg)
            {
                on_elbow_neg_button_released(widget, data);
                key_elbow_neg = FALSE;
            }
            break;
        case GDK_KEY_d:
            if(key_wrist_pos)
            {
                on_wrist_pos_button_released(widget, data);
                key_wrist_pos = FALSE;
            }
            break;
        case GDK_KEY_e:
            if(key_wrist_neg)
            {
                on_wrist_neg_button_released(widget, data);
                key_wrist_neg = FALSE;
            }
            break;
        case GDK_KEY_s:
            if(key_claw_pos)
            {
                on_claw_pos_button_released(widget, data);
                key_claw_pos = FALSE;
            }
            break;
        case GDK_KEY_w:
            if(key_claw_neg)
            {
                on_claw_neg_button_released(widget, data);
                key_claw_neg = FALSE;
            }
            break;
    }
    return FALSE;
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv); //initialising gtk - the gui lib i'm using

    //gui window setup
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //instantiating a window called window (so imaginative i know), TOPLEVEL allows window title etc
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


 //base label
    GtkWidget *base_label = gtk_label_new("base");
    gtk_box_pack_start(GTK_BOX(vbox), base_label, FALSE, FALSE, 0);

    //hbox for base buttons
    GtkWidget *base_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); //5 pixel spacing
    gtk_box_pack_start(GTK_BOX(vbox), base_hbox, FALSE, FALSE, 0);
    //base pos button (pressed and released for continuous holding button)
    GtkWidget *base_pos_button = gtk_button_new_with_label("+ (k)");
    g_signal_connect(base_pos_button, "pressed", G_CALLBACK(on_base_pos_button_pressed), NULL);
    g_signal_connect(base_pos_button, "released", G_CALLBACK(on_base_pos_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(base_hbox), base_pos_button, TRUE, TRUE, 0);
    //base neg button
    GtkWidget *base_neg_button = gtk_button_new_with_label("- (o)");
    g_signal_connect(base_neg_button, "pressed", G_CALLBACK(on_base_neg_button_pressed), NULL);
    g_signal_connect(base_neg_button, "released", G_CALLBACK(on_base_neg_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(base_hbox), base_neg_button, TRUE, TRUE, 0);


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
    //claw neg button (pressed and released for continuous holding button)
    GtkWidget *claw_neg_button = gtk_button_new_with_label("- (w)");
    g_signal_connect(claw_neg_button, "pressed", G_CALLBACK(on_claw_neg_button_pressed), NULL);
    g_signal_connect(claw_neg_button, "released", G_CALLBACK(on_claw_neg_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(claw_hbox), claw_neg_button, TRUE, TRUE, 0);

    //labels and buttons done
    gtk_widget_show_all(window);

    gtk_main(); //looping
    return 0;
}
