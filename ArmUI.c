#include <stdio.h>
#include <gtk/gtk.h> //for the gui
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/joystick.h>
#include <pthread.h>

#define JOYSTICK_DEV "/dev/input/js1"
#define AXIS_THRESHOLD 1000
#define DEVICE_PATH "/dev/A37JN_Robot_arm"

//holly checking if github working on vm :)

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

//status labels
GtkWidget *battery_label;
GtkWidget *arm_connection_label;
GtkWidget *joystick_connection_label;
GtkWidget *command_status_label;

//initialising statuses
int battery_status = 0;
char arm_connection_string[32] = "Arm status: disconnected";
char command_status_string[16] = "None";
char joystick_connection_string[32] = "Joystick status: disconnected";

void read_robot_status() {

    char buffer[512];

    const int fd = open(DEVICE_PATH, O_RDONLY);  // Open device file for writing
    if (fd == -1) {
        perror("Error opening device file");
        return;
    }

    const ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        perror("Error reading from device file");
        close(fd);
        return;
    }

    buffer[bytes_read] = '\0';

    char connected[4];
    char status[4];
    int battery = 0;

    if (sscanf(buffer, "connected:%3s status:%4s battery:%d", connected, status, &battery) != 3) {
        close(fd);
        return;
    }

    if (strcmp(status, "yes") == 0) {
        strcpy(arm_connection_string, "Arm status: Connected");
    } else {
        strcpy(arm_connection_string, "Arm status: Disconnected");
    }

    if (strcmp(status, "good") == 0) {
        strcpy(command_status_string, "Good");
    } else if (strcmp(status, "bad") == 0) {
        strcpy(command_status_string, "Bad");
    } else {
        strcpy(command_status_string, "None");
    }

    if (battery < 5 && battery > -1) {
        gchar *new_label_text = g_strdup_printf("Battery: %d/4", battery);
        gtk_label_set_text(GTK_LABEL(battery_label), new_label_text);
        g_free(new_label_text);  // Free allocated memory
    }

    close(fd);
}

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
    read_robot_status();
    close(fd);
    return 0;
}

//direct command input (ioctl)
static void on_text_entry_submit(GtkWidget *widget, gpointer data) {
    const gchar *input = gtk_entry_get_text(GTK_ENTRY(widget));
    printf("Debugging: received input: %s\n", input);
    //call to device.. would it use  send_robot_command() also?
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
    send_robot_command("base:right");
    printf("Debugging: base turning clockwise\n");
}

static void on_base_neg_button_pressed(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("base:left");
    printf("Debugging: base turning anticlockwise\n");
}
static void on_base_button_released(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("base:stop");
    printf("Debugging: base turning stopped\n");
}


//shoulder
static void on_shoulder_pos_button_pressed(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("shoulder:up");
    printf("Debugging: shoulder opening\n");
}

static void on_shoulder_neg_button_pressed(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("shoulder:down");
    printf("Debugging: shoulder closing\n");
}
static void on_shoulder_button_released(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("shoulder:stop");
    printf("Debugging: shoulder turning stopped\n");
}


//elbow
static void on_elbow_pos_button_pressed(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("elbow:up");
    printf("Debugging: elbow opening\n");
}
static void on_elbow_neg_button_pressed(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("elbow:down");
    printf("Debugging: elbow closing\n");
}
static void on_elbow_button_released(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("elbow:stop");
    printf("Debugging: elbow turning stopped\n");
}

//wrist 
static void on_wrist_pos_button_pressed(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("wrist:up");
    printf("Debugging: wrist opening\n");
}
static void on_wrist_neg_button_pressed(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("wrist:down");
    printf("Debugging: wrist closing\n");
}
static void on_wrist_button_released(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("wrist:stop");
    printf("Debugging: wrist turning stopped\n");
}

//claw (open = +, close = -)
static void on_claw_pos_button_pressed(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("claw:open");
    printf("Debugging: claw opening\n");
}
static void on_claw_neg_button_pressed(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("claw:close");
    printf("Debugging: claw closing\n");
}
static void on_claw_button_released(GtkWidget *widget, gpointer data) {
    //call to device
    send_robot_command("claw:stop");
    printf("Debugging: claw stopped\n");
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
                on_base_button_released(widget, data);
                key_base_pos = FALSE;
            }
            break;
        case GDK_KEY_o:
            if(key_base_neg)
            {
                on_base_button_released(widget, data);
                key_base_neg = FALSE;
            }
            break;
        case GDK_KEY_j:
            if(key_shoulder_pos)
            {
                on_shoulder_button_released(widget, data);
                key_shoulder_pos = FALSE;
            }
            break;
        case GDK_KEY_i:
            if(key_shoulder_neg)
            {
                on_shoulder_button_released(widget, data);
                key_shoulder_neg = FALSE;
            }
            break;
        case GDK_KEY_f:
            if(key_elbow_pos)
            {
                on_elbow_button_released(widget, data);
                key_elbow_pos = FALSE;
            }
            break;
        case GDK_KEY_r:
            if(key_elbow_neg)
            {
                on_elbow_button_released(widget, data);
                key_elbow_neg = FALSE;
            }
            break;
        case GDK_KEY_d:
            if(key_wrist_pos)
            {
                on_wrist_button_released(widget, data);
                key_wrist_pos = FALSE;
            }
            break;
        case GDK_KEY_e:
            if(key_wrist_neg)
            {
                on_wrist_button_released(widget, data);
                key_wrist_neg = FALSE;
            }
            break;
        case GDK_KEY_s:
            if(key_claw_pos)
            {
                on_claw_button_released(widget, data);
                key_claw_pos = FALSE;
            }
            break;
        case GDK_KEY_w:
            if(key_claw_neg)
            {
                on_claw_button_released(widget, data);
                key_claw_neg = FALSE;
            }
            break;
    }
    return FALSE;
}

void* joystick_listener(void *arg) {
    int fd = open(JOYSTICK_DEV, O_RDONLY);
    if (fd == -1) {
        perror("Error opening joystick");
        gtk_label_set_text(GTK_LABEL(joystick_connection_label), "Joystick: Disconnected");
        pthread_exit(NULL);
    } else {
        gtk_label_set_text(GTK_LABEL(joystick_connection_label), "Joystick status: Connected");
    }

    struct js_event js;
    printf("Joystick monitoring started on %s\n", JOYSTICK_DEV);

    while (1) {
        if (read(fd, &js, sizeof(struct js_event)) != sizeof(struct js_event)) {
            perror("Error reading joystick event");
            gtk_label_set_text(GTK_LABEL(joystick_connection_label), "Joystick disconnected");
            close(fd);
            pthread_exit(NULL);
        }

        if(js.type == JS_EVENT_BUTTON && js.value == 1) { // Button pressed
            switch (js.number) {
                case 0:
                    send_robot_command("claw:close");
                    printf("Claw Close\n");
                    break;

                case 1:
                    send_robot_command("claw:open");
                    printf("Claw opening\n");
                    break;

                case 2:
                    send_robot_command("wrist:down");
                    printf("Debugging: wrist down\n");
                    break;

                case 3:
                    send_robot_command("led:on");
                    printf("Joystick: Lights ON\n");
                    break;

                case 4:
                    send_robot_command("wrist:up");
                    printf("Debugging: wrist up\n");
                    break;
                
                case 5:
                    send_robot_command("led:off");
                    printf("Joystick: Lights OFF\n");
                    break;

                default:
                    printf("Joystick Button %d pressed\n", js.number);
                    break;
            }
        }

        if(js.type == JS_EVENT_AXIS) {
            switch(js.number) {
                case 1: // Shoulder tilt forward/backward
                    if(js.value == 32767) {
                        send_robot_command("shoulder:up");
                        printf("Axis 1: Shoulder UP\n");
                    } else if(js.value == -32767) {
                        send_robot_command("shoulder:down");
                        printf("Axis 1: Shoulder DOWN\n");
                    } else {
                        send_robot_command("shoulder:stop");
                        // printf("Axis 1: Shoulder stopped\n");
                    }
                    break;

                case 3: // Turning left/right
                    if(js.value == -32767) {
                        send_robot_command("base:left");
                        printf("Axis 3: Base rotating LEFT\n");
                    } else if(js.value == 32767) {
                        send_robot_command("base:right");
                        printf("Axis 3: Base rotating RIGHT\n");
                    } else {
                        send_robot_command("base:stop");
                        // printf("Axis 3: Base stopped\n");
                    }
                    break;

                case 4: // Elbow control
                    if(js.value == -32767) {
                        send_robot_command("elbow:up");
                        printf("Axis 4: Elbow UP\n");
                    } else if(js.value == 32767) {
                        send_robot_command("elbow:down");
                        printf("Axis 4: Elbow DOWN\n");
                    } else {
                        send_robot_command("elbow:stop");
                        // printf("Axis 4: Elbow stopped\n");
                    }
                    break;
            }
        }
        usleep(5000);
    }

    close(fd);
    return NULL;
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv); //initialising gtk - the gui lib i'm using

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //instantiating a window called window (so imaginative i know), TOPLEVEL allows window title etc
    gtk_window_set_title(GTK_WINDOW(window), "Robotic Arm Controller"); //window title
    gtk_container_set_border_width(GTK_CONTAINER(window), 20); //window border width size

    //how do i terminate the process when i close the window? signalssssss
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), NULL);
    g_signal_connect(window, "key-release-event", G_CALLBACK(on_key_release), NULL);
    gtk_widget_set_can_focus(window, TRUE);

    //create main horizontal box to split ui into two halves
    GtkWidget *hbox_main = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10); //10 pixel spacing
    gtk_container_add(GTK_CONTAINER(window), hbox_main);

    //left side vbox for mouse input buttons
    GtkWidget *vbox_left = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); //5 pixel spacing
    gtk_box_pack_start(GTK_BOX(hbox_main), vbox_left, TRUE, TRUE, 0);

    //right side vbox for text entry and status updates
    GtkWidget *vbox_right = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); //5 pixel spacing
    gtk_box_pack_start(GTK_BOX(hbox_main), vbox_right, TRUE, TRUE, 0);

    //light label
    GtkWidget *light_label = gtk_label_new("Light");
    gtk_box_pack_start(GTK_BOX(vbox_left), light_label, FALSE, FALSE, 0);

    //hbox for light buttons
    GtkWidget *light_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); //5 pixel spacing
    gtk_box_pack_start(GTK_BOX(vbox_left), light_hbox, FALSE, FALSE, 0);
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
    gtk_box_pack_start(GTK_BOX(vbox_left), base_label, FALSE, FALSE, 0);

    //hbox for base buttons
    GtkWidget *base_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); //5 pixel spacing
    gtk_box_pack_start(GTK_BOX(vbox_left), base_hbox, FALSE, FALSE, 0);
    //base pos button (pressed and released for continuous holding button)
    GtkWidget *base_pos_button = gtk_button_new_with_label("+ (k)");
    g_signal_connect(base_pos_button, "pressed", G_CALLBACK(on_base_pos_button_pressed), NULL);
    g_signal_connect(base_pos_button, "released", G_CALLBACK(on_base_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(base_hbox), base_pos_button, TRUE, TRUE, 0);
    //base neg button
    GtkWidget *base_neg_button = gtk_button_new_with_label("- (o)");
    g_signal_connect(base_neg_button, "pressed", G_CALLBACK(on_base_neg_button_pressed), NULL);
    g_signal_connect(base_neg_button, "released", G_CALLBACK(on_base_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(base_hbox), base_neg_button, TRUE, TRUE, 0);

    //shoulder label
    GtkWidget *shoulder_label = gtk_label_new("Shoulder");
    gtk_box_pack_start(GTK_BOX(vbox_left), shoulder_label, FALSE, FALSE, 0);

    //hbox for shoulder buttons
    GtkWidget *shoulder_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); //5 pixel spacing
    gtk_box_pack_start(GTK_BOX(vbox_left), shoulder_hbox, FALSE, FALSE, 0);
    //shoulder pos button (pressed and released for continuous holding button)
    GtkWidget *shoulder_pos_button = gtk_button_new_with_label("+ (j)");
    g_signal_connect(shoulder_pos_button, "pressed", G_CALLBACK(on_shoulder_pos_button_pressed), NULL);
    g_signal_connect(shoulder_pos_button, "released", G_CALLBACK(on_shoulder_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(shoulder_hbox), shoulder_pos_button, TRUE, TRUE, 0);
    //shoulder neg button
    GtkWidget *shoulder_neg_button = gtk_button_new_with_label("- (i)");
    g_signal_connect(shoulder_neg_button, "pressed", G_CALLBACK(on_shoulder_neg_button_pressed), NULL);
    g_signal_connect(shoulder_neg_button, "released", G_CALLBACK(on_shoulder_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(shoulder_hbox), shoulder_neg_button, TRUE, TRUE, 0);

    //elbow label
    GtkWidget *elbow_label = gtk_label_new("Elbow");
    gtk_box_pack_start(GTK_BOX(vbox_left), elbow_label, FALSE, FALSE, 0);

    //hbox for elbow buttons
    GtkWidget *elbow_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); //5 pixel spacing
    gtk_box_pack_start(GTK_BOX(vbox_left), elbow_hbox, FALSE, FALSE, 0);
    //elbow pos button (pressed and released for continuous holding button)
    GtkWidget *elbow_pos_button = gtk_button_new_with_label("+ (f)");
    g_signal_connect(elbow_pos_button, "pressed", G_CALLBACK(on_elbow_pos_button_pressed), NULL);
    g_signal_connect(elbow_pos_button, "released", G_CALLBACK(on_elbow_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(elbow_hbox), elbow_pos_button, TRUE, TRUE, 0);
    //elbow neg button
    GtkWidget *elbow_neg_button = gtk_button_new_with_label("- (r)");
    g_signal_connect(elbow_neg_button, "pressed", G_CALLBACK(on_elbow_neg_button_pressed), NULL);
    g_signal_connect(elbow_neg_button, "released", G_CALLBACK(on_elbow_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(elbow_hbox), elbow_neg_button, TRUE, TRUE, 0);

    //wrist label
    GtkWidget *wrist_label = gtk_label_new("Wrist");
    gtk_box_pack_start(GTK_BOX(vbox_left), wrist_label, FALSE, FALSE, 0);

    //hbox for wrist buttons
    GtkWidget *wrist_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); //5 pixel spacing
    gtk_box_pack_start(GTK_BOX(vbox_left), wrist_hbox, FALSE, FALSE, 0);
    //wrist pos button (pressed and released for continuous holding button)
    GtkWidget *wrist_pos_button = gtk_button_new_with_label("+ (d)");
    g_signal_connect(wrist_pos_button, "pressed", G_CALLBACK(on_wrist_pos_button_pressed), NULL);
    g_signal_connect(wrist_pos_button, "released", G_CALLBACK(on_wrist_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(wrist_hbox), wrist_pos_button, TRUE, TRUE, 0);
    //wrist neg button
    GtkWidget *wrist_neg_button = gtk_button_new_with_label("- (e)");
    g_signal_connect(wrist_neg_button, "pressed", G_CALLBACK(on_wrist_neg_button_pressed), NULL);
    g_signal_connect(wrist_neg_button, "released", G_CALLBACK(on_wrist_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(wrist_hbox), wrist_neg_button, TRUE, TRUE, 0);

    //claw label
    GtkWidget *claw_label = gtk_label_new("Claw");
    gtk_box_pack_start(GTK_BOX(vbox_left), claw_label, FALSE, FALSE, 0);

    //hbox for claw buttons
    GtkWidget *claw_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); //5 pixel spacing
    gtk_box_pack_start(GTK_BOX(vbox_left), claw_hbox, FALSE, FALSE, 0);
    //claw pos button (pressed and released for continuous holding button)
    GtkWidget *claw_pos_button = gtk_button_new_with_label("+ (s)");
    g_signal_connect(claw_pos_button, "pressed", G_CALLBACK(on_claw_pos_button_pressed), NULL);
    g_signal_connect(claw_pos_button, "released", G_CALLBACK(on_claw_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(claw_hbox), claw_pos_button, TRUE, TRUE, 0);
    //claw neg button (pressed and released for continuous holding button)
    GtkWidget *claw_neg_button = gtk_button_new_with_label("- (w)");
    g_signal_connect(claw_neg_button, "pressed", G_CALLBACK(on_claw_neg_button_pressed), NULL);
    g_signal_connect(claw_neg_button, "released", G_CALLBACK(on_claw_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(claw_hbox), claw_neg_button, TRUE, TRUE, 0);

    //right side vbox
    //status update: battery status
    //update/check battery
    battery_label = gtk_label_new("Battery: 0/4");
    gtk_box_pack_start(GTK_BOX(vbox_right), battery_label, FALSE, FALSE, 0);
    gtk_widget_set_halign(battery_label, GTK_ALIGN_START);
    
    //label for ioctl input field
    GtkWidget *ioctl_label = gtk_label_new("IOCTL command input:");
    gtk_box_pack_start(GTK_BOX(vbox_right), ioctl_label, FALSE, FALSE, 0);
    gtk_widget_set_halign(ioctl_label, GTK_ALIGN_START);

    //text entry for ioctl input
    GtkWidget *text_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(text_entry), "Enter direct command...");
    g_signal_connect(text_entry, "activate", G_CALLBACK(on_text_entry_submit), NULL);
    gtk_box_pack_start(GTK_BOX(vbox_right), text_entry, FALSE, FALSE, 0);
    gtk_widget_set_halign(text_entry, GTK_ALIGN_START);

    //label for command status
    GtkWidget *command_status_label = gtk_label_new("Command status: none");
    gtk_box_pack_start(GTK_BOX(vbox_right), command_status_label, FALSE, FALSE, 0);
    gtk_widget_set_halign(command_status_label, GTK_ALIGN_START);


    //status update: arm connection status
    //update/check arm connection
    arm_connection_label = gtk_label_new("Arm status: disconnected");
    gtk_box_pack_start(GTK_BOX(vbox_right), arm_connection_label, FALSE, FALSE, 0);
    gtk_widget_set_halign(arm_connection_label, GTK_ALIGN_START);
    
    //status update: external input status
    //update/check joystick connection
    joystick_connection_label = gtk_label_new(joystick_connection_string);
    gtk_box_pack_start(GTK_BOX(vbox_right), joystick_connection_label, FALSE, FALSE, 0);
    gtk_widget_set_halign(joystick_connection_label, GTK_ALIGN_START);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

