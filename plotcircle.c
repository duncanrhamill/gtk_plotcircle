#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>

// Widgets to be accessible from any function
static GtkWidget *canvas, *radius, *xpos, *ypos;

// Draw callback run every time the drawing area is queued for a re-draw
static void draw(GtkWidget *widget, cairo_t * cr, gpointer data) {
    // Centre the cairo surface on the middle of the drawing area
    cairo_translate(cr, 300, 300);

    // Clear the previous frame
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);

    // Extract the x-pos, y-pos, and radius from the entry fields
    double xc;
    sscanf(gtk_entry_get_text(GTK_ENTRY(xpos)), "%lf", &xc);

    double yc;
    sscanf(gtk_entry_get_text(GTK_ENTRY(ypos)), "%lf", &yc);
    yc = -yc;

    double r;
    sscanf(gtk_entry_get_text(GTK_ENTRY(radius)), "%lf", &r);

    // Draw the circle
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_arc(cr, xc, yc, r, 0, 2 * M_PI);
    cairo_set_line_width(cr, 2);
    cairo_stroke(cr);

    // Draw the origin crosshair
    cairo_move_to(cr, -25, 0);
    cairo_line_to(cr, 25, 0);
    cairo_move_to(cr, 0, 25);
    cairo_line_to(cr, 0, -25);
    cairo_set_line_width(cr, 1);
    cairo_stroke(cr);
}

// Reset the circle to the centre with radius 100
static void trigger_reset(GtkWidget *widget, gpointer data) {
    gtk_entry_set_text(GTK_ENTRY(xpos), "0");
    gtk_entry_set_text(GTK_ENTRY(ypos), "0");
    gtk_entry_set_text(GTK_ENTRY(radius), "100");
    gtk_widget_queue_draw(canvas);
}

// When the plot button is pressed queue up the draw area to be re-drawn
static void trigger_draw(GtkWidget *widget, gpointer data) {
    gtk_widget_queue_draw(canvas);
}

// Entry point for the application
int main(int argc, char ** argv) {
    // Create the window, upper level grid and plot button
    GtkWidget *window, *grid, *btn_plot, *btn_reset, *rlabel, *xlabel, *ylabel;
    gtk_init(&argc, &argv);

    // Create the window object and set the window size
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "PlotCircle");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // Main grid
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 12);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Drawing canvas
    canvas = gtk_drawing_area_new();
    gtk_widget_set_size_request(canvas, 600, 600);
    g_signal_connect(canvas, "draw", G_CALLBACK(draw), NULL);
    gtk_grid_attach(GTK_GRID(grid), canvas, 0, 0, 1, 4);

    // Plot button
    btn_plot = gtk_button_new_with_label("Plot");
    gtk_widget_set_halign(btn_plot, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(btn_plot, GTK_ALIGN_CENTER);
    gtk_widget_set_tooltip_text(btn_plot, "Re-plot the circle");
    g_signal_connect(btn_plot, "clicked", G_CALLBACK(trigger_draw), NULL);
    gtk_grid_attach(GTK_GRID(grid), btn_plot, 1, 3, 1, 1);

    // Reset button
    btn_reset = gtk_button_new_with_label("Reset");
    gtk_widget_set_halign(btn_reset, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(btn_reset, GTK_ALIGN_CENTER);
    gtk_widget_set_tooltip_text(btn_reset, "Reset the screen to original position");
    g_signal_connect(btn_reset, "clicked", G_CALLBACK(trigger_reset), NULL);
    gtk_grid_attach(GTK_GRID(grid), btn_reset, 2, 3, 1, 1);

    // X position field
    xpos = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(xpos), "0");
    gtk_widget_set_valign(xpos, GTK_ALIGN_CENTER);
    g_signal_connect(xpos, "activate", G_CALLBACK(trigger_draw), NULL);
    gtk_grid_attach(GTK_GRID(grid), xpos, 2, 0, 1, 1);
    xlabel = gtk_label_new("x position:");
    gtk_grid_attach(GTK_GRID(grid), xlabel, 1, 0, 1, 1);

    // Y position field
    ypos = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(ypos), "0");
    gtk_widget_set_valign(ypos, GTK_ALIGN_CENTER);
    g_signal_connect(ypos, "activate", G_CALLBACK(trigger_draw), NULL);
    gtk_grid_attach(GTK_GRID(grid), ypos, 2, 1, 1, 1);
    ylabel = gtk_label_new("y position:");
    gtk_grid_attach(GTK_GRID(grid), ylabel, 1, 1, 1, 1);

    // Radius field
    radius = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(radius), "100");
    gtk_widget_set_valign(radius, GTK_ALIGN_CENTER);
    g_signal_connect(radius, "activate", G_CALLBACK(trigger_draw), NULL);
    gtk_grid_attach(GTK_GRID(grid), radius, 2, 2, 1, 1);
    rlabel = gtk_label_new("radius:");
    gtk_grid_attach(GTK_GRID(grid), rlabel, 1, 2, 1, 1);

    // Show all widgets
    gtk_widget_show_all(window);

    // Attach the destroy signal
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Enter the main loop and await events
    gtk_main();

    return 0;
}