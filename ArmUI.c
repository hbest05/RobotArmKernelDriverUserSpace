#include <stdio.h>
#include <gtk/gtk.h> //for the gui
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/joystick.h> // for joystick
#include <pthread.h>
#include <stdbool.h>

// Path to robot arm
#define DEVICE_PATH "/dev/A37JN_Robot_arm"
#define MAGIC_NUM 0x80
#define IOCTL_SET_VALUE _IOW(MAGIC_NUM, 1, struct device_command)

// Depends on system (change to "js0" or "js1")
#define JOYSTICK_DEV "/dev/input/js1"
#define AXIS_THRESHOLD 1000

//isolating input, default to mouse (0), with keyboard(2), and joystick (3)
static int active_input_mode = 0;
static GList *control_buttons = NULL; 

static bool keyboard_enabled = TRUE;
static bool joystick_enabled = TRUE;

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
GtkWidget *battery_status_label;
GtkWidget *arm_connection_label;
GtkWidget *joystick_connection_label;
GtkWidget *command_status_label;

//initialising statuses
int battery_status = 0;

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

    //printf("%s", buffer);

    char connected[4];
    char status[4];
    char battery[2];

    if (sscanf(buffer, "connected:%4s status:%4s battery:%1s", connected, status, battery) != 3) {
        close(fd);
        return;
    }

    close(fd);

    // Actually convert String to int the proper way
    const int battery_level = (int) strtol(battery, NULL, 10);

    //printf("connected:%s status:%s battery:%d\n", connected, status, battery_level);

    char *final_connected_string;  // Declare outside

    if (strcmp(connected, "yes") == 0) {
        final_connected_string = "Connected";
    } else {
        final_connected_string = "Disconnected";
    }

    gtk_label_set_text(GTK_LABEL(arm_connection_label),
        g_strdup_printf("Arm status: %s", final_connected_string));

    char *final_Command_String;

    if (strcmp(status, "good") == 0) {
        final_Command_String = "Good";
    } else if (strcmp(status, "bad") == 0) {
        final_Command_String = "Bad";
    } else {
        final_Command_String = "None";
    }

    gtk_label_set_text(GTK_LABEL(command_status_label),
        g_strdup_printf("Command status: %s", final_Command_String));

    if (battery_level < 5 && battery_level > -1) {
        gtk_label_set_text(GTK_LABEL(battery_status_label),
         g_strdup_printf("Battery: %d/4", battery_level));
    }

}

/**
 * Sends a command to the A37JN robot arm via the device file.
 * @param command: The command string to send (e.g., "base:left\n").
 * @return 0 on success, -1 on failure.
 */

int send_robot_command(const char *command) {

    printf("Sending command: %s\n", command);

    const int fd = open(DEVICE_PATH, O_WRONLY);  // Open device file for writing
    if (fd < 0) {
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

struct device_command {
    int var1;
    int var2;
    int var3;
};

//direct command input (ioctl)
static void on_text_entry_submit(GtkWidget *widget, gpointer data) {

    GtkWidget *entry = GTK_WIDGET(data);
    const gchar *input = gtk_entry_get_text(GTK_ENTRY(entry));
    printf("Debugging: received input: %s\n", input);

    struct device_command cmd;

    // Parse input (Expected format: "128,2,0")
    if (sscanf(input, "%d,%d,%d", &cmd.var1, &cmd.var2, &cmd.var3) != 3) {
        printf("Error: Invalid input format. Expected: var1,var2,var3\n");
        return;
    }

    const int fd = open(DEVICE_PATH, O_WRONLY);  // Open device file for writing
    if (fd < 0) {
        perror("Error opening device file");
        return;
    }

    printf("Debugging: Sending command: %d,%d,%d\n", cmd.var1, cmd.var2, cmd.var3);

    if (ioctl(fd, IOCTL_SET_VALUE, &cmd) == -1) {
        perror("ioctl failed");
        gtk_label_set_text(GTK_LABEL(command_status_label),"Command status: Bad");
        close(fd);
        return;
    }

    printf("Sent ioctl command: var1=%d, var2=%d, var3=%d\n", cmd.var1, cmd.var2, cmd.var3);

    close(fd);

    //call to device... would it use  send_robot_command() also?
    gtk_entry_set_text(GTK_ENTRY(entry), "");

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
static gboolean on_key_press(GtkWidget *widget, const GdkEventKey *event, gpointer data) {

    if (active_input_mode != 1) return FALSE;

    // Implemented keyboard block
    if (!keyboard_enabled) {
        return FALSE;
    }

    switch (event->keyval) {

        case GDK_KEY_1:
            if(!key_light_on) {
                key_light_on = TRUE;
                on_light_on_button_clicked(widget, data);
            }
            break;

        case GDK_KEY_2:
            if(!key_light_off) {
                key_light_off = TRUE;
                on_light_off_button_clicked(widget, data);
            }
            break;

        case GDK_KEY_k:
            if(!key_base_pos) {
                key_base_pos = TRUE;
                on_base_pos_button_pressed(widget, data);
            }
            break;

        case GDK_KEY_o:
            if(!key_base_neg) {
                key_base_neg = TRUE;
                on_base_neg_button_pressed(widget, data);
            }
            break;

        case GDK_KEY_j:
            if(!key_shoulder_pos) {
                key_shoulder_pos = TRUE;
                on_shoulder_pos_button_pressed(widget, data);
            }
            break;

        case GDK_KEY_i:
            if(!key_shoulder_neg) {
                key_shoulder_neg = TRUE;
                on_shoulder_neg_button_pressed(widget, data);
            }
            break;

        case GDK_KEY_f:
            if(!key_elbow_pos) {
                key_elbow_pos = TRUE;
                on_elbow_pos_button_pressed(widget, data);
            }
            break;

        case GDK_KEY_r:
            if(!key_elbow_neg) {
                key_elbow_neg = TRUE;
                on_elbow_neg_button_pressed(widget, data);
            }
            break;

        case GDK_KEY_d:
            if(!key_wrist_pos) {
                key_wrist_pos = TRUE;
                on_wrist_pos_button_pressed(widget, data);
            }
            break;

        case GDK_KEY_e:
            if(!key_wrist_neg) {
                key_wrist_neg = TRUE;
                on_wrist_neg_button_pressed(widget, data);
            }
            break;

        case GDK_KEY_s:
            if(!key_claw_pos) {
                key_claw_pos = TRUE;
                on_claw_pos_button_pressed(widget, data);
            }
            break;

        case GDK_KEY_w:
            if(!key_claw_neg) {
                key_claw_neg = TRUE;
                on_claw_neg_button_pressed(widget, data);
            }
            break;

        default:
            break;
    }
    return FALSE;
}

//key release event callback
static void on_key_release(GtkWidget *widget, const GdkEventKey *event, gpointer data) {

    if (active_input_mode != 1) return;

    switch (event->keyval) {
        case GDK_KEY_1:
            key_light_on = FALSE;
            break;

        case GDK_KEY_2:
            key_light_off = FALSE;
            break;

        case GDK_KEY_k:
            if(key_base_pos) {
                on_base_button_released(widget, data);
                key_base_pos = FALSE;
            }
            break;

        case GDK_KEY_o:
            if(key_base_neg) {
                on_base_button_released(widget, data);
                key_base_neg = FALSE;
            }
            break;

        case GDK_KEY_j:
            if(key_shoulder_pos) {
                on_shoulder_button_released(widget, data);
                key_shoulder_pos = FALSE;
            }
            break;

        case GDK_KEY_i:
            if(key_shoulder_neg) {
                on_shoulder_button_released(widget, data);
                key_shoulder_neg = FALSE;
            }
            break;

        case GDK_KEY_f:
            if(key_elbow_pos) {
                on_elbow_button_released(widget, data);
                key_elbow_pos = FALSE;
            }
            break;

        case GDK_KEY_r:
            if(key_elbow_neg) {
                on_elbow_button_released(widget, data);
                key_elbow_neg = FALSE;
            }
            break;

        case GDK_KEY_d:
            if(key_wrist_pos) {
                on_wrist_button_released(widget, data);
                key_wrist_pos = FALSE;
            }
            break;

        case GDK_KEY_e:
            if(key_wrist_neg) {
                on_wrist_button_released(widget, data);
                key_wrist_neg = FALSE;
            }
            break;

        case GDK_KEY_s:
            if(key_claw_pos) {
                on_claw_button_released(widget, data);
                key_claw_pos = FALSE;
            }
            break;

        case GDK_KEY_w:
            if(key_claw_neg) {
                on_claw_button_released(widget, data);
                key_claw_neg = FALSE;
            }
            break;

        default:
            break;
    }
}

void* joystick_listener(void *arg) {

    int fd = open(JOYSTICK_DEV, O_RDONLY);

    if (fd < 0) {
        perror("Error opening joystick");
        gtk_label_set_text(GTK_LABEL(joystick_connection_label), "Joystick status: Disconnected");
        printf("Attempting to connect to joystick...\n");

        // Always tries to detect the Joystick
        while (fd < 0) {
            usleep(10000);
            fd = open(JOYSTICK_DEV, O_RDONLY);
        }
    }

    // UI showing the Joystick is connected
    gtk_label_set_text(GTK_LABEL(joystick_connection_label), "Joystick status: Connected");

    struct js_event js;
    printf("Joystick monitoring started on %s\n", JOYSTICK_DEV);
        
    // Initalising the states for all the axis motors
    static int last_wrist_state = 0;
    static int last_shoulder_state = 0;
    static int last_rotate_state = 0;
    static int last_elbow_state = 0;
    static int last_claw_state = 0;

while (joystick_enabled) {

        if (active_input_mode != 2) {
            usleep(10000);
            continue;
        }

        if (read(fd, &js, sizeof(struct js_event)) != sizeof(struct js_event)) {

            gtk_label_set_text(GTK_LABEL(joystick_connection_label), "Joystick status: Disconnected");
            printf("Attempting to reconnect to joystick...\n");

            // Wait for joystick to be reconnected
            close(fd);
            fd = -1;

            // Will keep trying to detect Joystick
            while (fd < 0) {
                usleep(10000);
                fd = open(JOYSTICK_DEV, O_RDONLY);
            }

            gtk_label_set_text(GTK_LABEL(joystick_connection_label), "Joystick status: Connected");
            continue;

        }

        if(js.type == JS_EVENT_BUTTON) { 
            if(js.value == 1) { 
                switch (js.number) {
                    case 0:
                        if (last_claw_state != -1) {
                            send_robot_command("claw:close");
                            printf("Claw Close\n");
                            last_claw_state = -1;
                        }
                        break;
        
                    case 1:
                        if (last_claw_state != 1) {
                            send_robot_command("claw:open");
                            last_claw_state = 1;
                            printf("Claw opening\n");
                        }
                        break;
        
                    case 2:
                        if(last_wrist_state != -1) {
                            send_robot_command("wrist:down");
                            last_wrist_state = -1;
                            printf("Debugging: wrist down\n");
                        }
                        break;
        
                    case 3:
                        send_robot_command("led:off");
                        printf("Joystick: Lights off\n");
                        break;
        
                    case 4:
                        if(last_wrist_state != 1) {
                            send_robot_command("wrist:up");
                            last_wrist_state = 1;
                            printf("Debugging: wrist up\n");
                        }
                        break;
        
                    case 5:
                        send_robot_command("led:on");
                        printf("Joystick: Lights on\n");
                        break;
        
                    default:
                        printf("Joystick Button %d pressed\n", js.number);
                        break;
                }

            } else if(js.value == 0) { // Button released
                switch (js.number) {

                    // Two cases to handle both buttons being released
                    case 0:
                    case 1:
                        if (last_claw_state != 0) {
                            send_robot_command("claw:stop");
                            last_claw_state = 0;
                            printf("Claw stopped\n");
                        }
                        break;

                    case 2: // Wrist down button released
                    case 4: // Wrist up button released
                        if(last_wrist_state != 0) {
                            send_robot_command("wrist:stop");
                            last_wrist_state = 0;
                            printf("Debugging: wrist stopped\n");
                        }
                        break;

                    default:
                        printf("Joystick Button %d released\n", js.number);
                        break;
                }
            }
        }

        if(js.type == JS_EVENT_AXIS) {

            const int dead_zone = 10000;

            switch(js.number) {
                case 1: { // Shoulder tilt (forward/backward)
                    int new_state;
        
                    if(js.value >= dead_zone) {
                        new_state = 1; // Moving up
                    } else if(js.value <= -dead_zone) {
                        new_state = -1; // Moving down
                    } else {
                        new_state = 0; // Stopped
                    }
        
                    if(new_state != last_shoulder_state) {
                        last_shoulder_state = new_state;
                        if(new_state == 1) {
                            send_robot_command("shoulder:up");
                            printf("Axis 1: Shoulder UP\n");
                        } else if(new_state == -1) {
                            send_robot_command("shoulder:down");
                            printf("Axis 1: Shoulder DOWN\n");
                        } else {
                            send_robot_command("shoulder:stop");
                            printf("Axis 1: Shoulder stopped\n");
                        }
                    }
                    break;
                }
                case 3: { // Base logic (left/right)
                    int new_state;

                    // Extra dead-zone for the base as it's needed
                    if(js.value >= (dead_zone + 10000*2 )) {
                        new_state = 1; // Turning Left
                    } else if(js.value <= -(dead_zone + 10000 )) {
                        new_state = -1; // Turning Right
                    } else {
                        new_state = 0; // Stopped
                    }
        
                    if(new_state != last_rotate_state) {
                        last_rotate_state = new_state;
                        if(new_state == 1) {
                            send_robot_command("base:right");
                            printf("Axis 3: Positive\n");
                        } else if(new_state == -1) {
                            send_robot_command("base:left");
                            printf("Axis 3: Negative\n");
                        } else {
                            send_robot_command("base:stop");
                            printf("Axis 3: stopped\n");
                        }
                    }
                    break;
                }
                case 5: { // Wrist Logic
                    int new_state;
        
                    if(js.value >= dead_zone) {
                        new_state = 1; // Moving up
                    } else if(js.value <= -dead_zone) {
                        new_state = -1; // Moving down
                    } else {
                        new_state = 0; // Stopped
                    }
        
                    if(new_state != last_elbow_state) {
                        last_elbow_state = new_state;
                        if(new_state == 1) {
                            send_robot_command("elbow:down");
                            printf("Axis 4: Positive\n");
                        } else if(new_state == -1) {
                            send_robot_command("elbow:up");
                            printf("Axis 4: Negative\n");
                        } else {
                            send_robot_command("elbow:stop");
                            printf("Axis 4: stopped\n");
                        }   
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
    close(fd);
    return NULL;
}

static void toggle_buttons(gboolean enable) {
    GList *i;
    for (i = control_buttons; i != NULL; i = i->next) {
        gtk_widget_set_sensitive(GTK_WIDGET(i->data), enable);
    }
}

static void on_mouse_toggle_clicked(GtkWidget *widget, gpointer data) {
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
        active_input_mode = 0;
        keyboard_enabled = FALSE;
        toggle_buttons(TRUE);
    }
}

static void on_keyboard_toggle_clicked(GtkWidget *widget, gpointer data) {
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
        active_input_mode = 1;
        keyboard_enabled = TRUE;
        toggle_buttons(FALSE);
    }
}

static void on_joystick_toggle_clicked(GtkWidget *widget, gpointer data) {
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
        active_input_mode = 2;
        keyboard_enabled = FALSE;
        toggle_buttons(FALSE);

    }
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv); //initialising gtk - the gui lib i'm using

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //instantiating a window called window (so imaginative I know), TOPLEVEL allows window title etc
    gtk_window_set_title(GTK_WINDOW(window), "Robotic Arm Controller"); //window title
    gtk_container_set_border_width(GTK_CONTAINER(window), 20); //window border width size

    //how do I terminate the process when I close the window? signals
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
    gtk_widget_set_margin_start(vbox_right, 20);

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
    GtkWidget *base_pos_button = gtk_button_new_with_label("Right (k)");
    g_signal_connect(base_pos_button, "pressed", G_CALLBACK(on_base_pos_button_pressed), NULL);
    g_signal_connect(base_pos_button, "released", G_CALLBACK(on_base_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(base_hbox), base_pos_button, TRUE, TRUE, 0);
    //base neg button
    GtkWidget *base_neg_button = gtk_button_new_with_label("Left (o)");
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
    GtkWidget *shoulder_pos_button = gtk_button_new_with_label("Up (j)");
    g_signal_connect(shoulder_pos_button, "pressed", G_CALLBACK(on_shoulder_pos_button_pressed), NULL);
    g_signal_connect(shoulder_pos_button, "released", G_CALLBACK(on_shoulder_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(shoulder_hbox), shoulder_pos_button, TRUE, TRUE, 0);
    //shoulder neg button
    GtkWidget *shoulder_neg_button = gtk_button_new_with_label("Down (i)");
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
    GtkWidget *elbow_pos_button = gtk_button_new_with_label("Up (f)");
    g_signal_connect(elbow_pos_button, "pressed", G_CALLBACK(on_elbow_pos_button_pressed), NULL);
    g_signal_connect(elbow_pos_button, "released", G_CALLBACK(on_elbow_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(elbow_hbox), elbow_pos_button, TRUE, TRUE, 0);
    //elbow neg button
    GtkWidget *elbow_neg_button = gtk_button_new_with_label("Down (r)");
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
    GtkWidget *wrist_pos_button = gtk_button_new_with_label("Up (d)");
    g_signal_connect(wrist_pos_button, "pressed", G_CALLBACK(on_wrist_pos_button_pressed), NULL);
    g_signal_connect(wrist_pos_button, "released", G_CALLBACK(on_wrist_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(wrist_hbox), wrist_pos_button, TRUE, TRUE, 0);
    //wrist neg button
    GtkWidget *wrist_neg_button = gtk_button_new_with_label("Down (e)");
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
    GtkWidget *claw_pos_button = gtk_button_new_with_label("Open (s)");
    g_signal_connect(claw_pos_button, "pressed", G_CALLBACK(on_claw_pos_button_pressed), NULL);
    g_signal_connect(claw_pos_button, "released", G_CALLBACK(on_claw_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(claw_hbox), claw_pos_button, TRUE, TRUE, 0);
    //claw neg button (pressed and released for continuous holding button)
    GtkWidget *claw_neg_button = gtk_button_new_with_label("Close (w)");
    g_signal_connect(claw_neg_button, "pressed", G_CALLBACK(on_claw_neg_button_pressed), NULL);
    g_signal_connect(claw_neg_button, "released", G_CALLBACK(on_claw_button_released), NULL);
    gtk_box_pack_start(GTK_BOX(claw_hbox), claw_neg_button, TRUE, TRUE, 0);

    //right side vbox

    //list of buttons for isolating mouse input
    control_buttons = g_list_append(control_buttons, light_on_button);
    control_buttons = g_list_append(control_buttons, light_off_button);
    control_buttons = g_list_append(control_buttons, base_pos_button);
    control_buttons = g_list_append(control_buttons, base_neg_button);
    control_buttons = g_list_append(control_buttons, shoulder_pos_button);
    control_buttons = g_list_append(control_buttons, shoulder_neg_button);
    control_buttons = g_list_append(control_buttons, elbow_pos_button);
    control_buttons = g_list_append(control_buttons, elbow_neg_button);
    control_buttons = g_list_append(control_buttons, wrist_pos_button);
    control_buttons = g_list_append(control_buttons, wrist_neg_button);
    control_buttons = g_list_append(control_buttons, claw_pos_button);
    control_buttons = g_list_append(control_buttons, claw_neg_button);

    //input isolating toggle buttons
    GtkWidget *input_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox_right), input_hbox, FALSE, FALSE, 0);

    //using radio buttons for mutually exclusive selection
    GtkWidget *mouse_toggle = gtk_radio_button_new_with_label(NULL, "Mouse input");
    GtkWidget *keyboard_toggle = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(mouse_toggle), "Keyboard input");
    GtkWidget *joystick_toggle = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(mouse_toggle), "Joystick input");

    //default selection
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mouse_toggle), TRUE);

    gtk_box_pack_start(GTK_BOX(input_hbox), mouse_toggle, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(input_hbox), keyboard_toggle, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(input_hbox), joystick_toggle, TRUE, TRUE, 0);

    g_signal_connect(mouse_toggle, "toggled", G_CALLBACK(on_mouse_toggle_clicked), NULL);
    g_signal_connect(keyboard_toggle, "toggled", G_CALLBACK(on_keyboard_toggle_clicked), NULL);
    g_signal_connect(joystick_toggle, "toggled", G_CALLBACK(on_joystick_toggle_clicked), NULL);

    gtk_widget_set_margin_bottom(input_hbox, 10);
    
    ///label for ioctl input field
    GtkWidget *ioctl_label = gtk_label_new("IOCTL command input:");
    gtk_box_pack_start(GTK_BOX(vbox_right), ioctl_label, FALSE, FALSE, 0);
    gtk_widget_set_halign(ioctl_label, GTK_ALIGN_START);

    //hbox for both the text entry and send button
    GtkWidget *ioctl_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox_right), ioctl_hbox, FALSE, FALSE, 0);
    gtk_box_set_homogeneous(GTK_BOX(ioctl_hbox), FALSE); //keeps send button beside textbox

    gtk_widget_set_halign(ioctl_hbox, GTK_ALIGN_START);

    //text entry for ioctl input
    GtkWidget *text_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(text_entry), "Enter direct command...");
    g_signal_connect(text_entry, "activate", G_CALLBACK(on_text_entry_submit), text_entry);
    gtk_entry_set_width_chars(GTK_ENTRY(text_entry), 22); //textbox width
    gtk_box_pack_start(GTK_BOX(ioctl_hbox), text_entry, FALSE, FALSE, 0);
    gtk_widget_set_hexpand(text_entry, TRUE);
    gtk_widget_set_margin_top(text_entry, 5);
    
    //send button for ioctl input
    GtkWidget *send_button = gtk_button_new_with_label("Send");
    g_signal_connect(send_button, "clicked", G_CALLBACK(on_text_entry_submit), text_entry);
    gtk_box_pack_start(GTK_BOX(ioctl_hbox), send_button, FALSE, FALSE, 0);
    gtk_widget_set_margin_top(send_button, 5);

    gtk_widget_set_margin_bottom(ioctl_hbox, 10);

    //label for command status
    command_status_label = gtk_label_new("Command status: None");
    gtk_box_pack_start(GTK_BOX(vbox_right), command_status_label, FALSE, FALSE, 0);
    gtk_widget_set_halign(command_status_label, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(command_status_label, 10);

    //status update: arm connection status
    //update/check arm connection
    arm_connection_label = gtk_label_new("Arm status: Disconnected");
    gtk_box_pack_start(GTK_BOX(vbox_right), arm_connection_label, FALSE, FALSE, 0);
    gtk_widget_set_halign(arm_connection_label, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(arm_connection_label, 10);

    //status update: external input status
    //update/check joystick connection
    joystick_connection_label = gtk_label_new("Joystick status: Disconnected");
    gtk_box_pack_start(GTK_BOX(vbox_right), joystick_connection_label, FALSE, FALSE, 0);
    gtk_widget_set_halign(joystick_connection_label, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(joystick_connection_label, 10);

    //status update: battery status
    //update/check battery
    battery_status_label = gtk_label_new("Battery: 0/4");
    gtk_box_pack_start(GTK_BOX(vbox_right), battery_status_label, FALSE, FALSE, 0);
    gtk_widget_set_halign(battery_status_label, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(battery_status_label, 10);

    // Declare a thread variable for the joystick listener
    pthread_t joystick_thread;

    // Attempt to create a new thread
    if (pthread_create(&joystick_thread, NULL, joystick_listener, NULL) != 0) {
        // If thread creation fails, print an error message
        perror("Failed to create joystick thread");

         // Update the GTK label to indicate the joystick connection failed
        gtk_label_set_text(GTK_LABEL(joystick_connection_label), "Joystick status: Failed");
    } else {
        // If the thread is successfully created, update the label to show connection success
        gtk_label_set_text(GTK_LABEL(joystick_connection_label), "Joystick status: Connected");
    }

    gtk_widget_show_all(window);

    // Send this to make sure arm is not moving and to show connection status
    send_robot_command("stop:all");
    gtk_label_set_text(GTK_LABEL(command_status_label),"Command status: None");

    gtk_main();

    return 0;
}
