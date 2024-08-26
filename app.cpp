#include <iostream>
#include <gtk-3.0/gtk/gtk.h>
#include "scp.h"

GtkWidget *win, *grid, *chooseFileLabel, *fileLocationEntry, *fileChoosebutton, *title, *warning, *hostNameLabel, *hostPasswordLabel, *hostIpLabel, *hostNameEntry, *hostPasswordEntry, *sendButton, *hostIpEntry, *clearButton;
std::string fileToSend = "";

void clearAll()
{
  gtk_entry_set_text(GTK_ENTRY(fileLocationEntry), "");
  gtk_entry_set_text(GTK_ENTRY(hostIpEntry), "");
  gtk_entry_set_text(GTK_ENTRY(hostNameEntry), "");
  gtk_entry_set_text(GTK_ENTRY(hostPasswordEntry), "");
}

std::string selectFile(GtkWidget *w, gpointer data)
{
  GtkWidget *dialog;
  dialog = gtk_file_chooser_dialog_new("Choose a file", GTK_WINDOW(win),
  GTK_FILE_CHOOSER_ACTION_OPEN,
  GTK_STOCK_OK,
  GTK_RESPONSE_OK,
  GTK_STOCK_CANCEL,
  GTK_RESPONSE_CANCEL,
  NULL
  );
  gtk_widget_show_all(dialog);
  gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), g_get_home_dir());
  gint res = gtk_dialog_run(GTK_DIALOG(dialog));
  if (res = GTK_RESPONSE_OK)
  {
    fileToSend = "\"" + (std::string)gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)) + "\"";
    g_print("%s\n", gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
    gtk_entry_set_text(GTK_ENTRY(fileLocationEntry), gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
  }
  else
    std::cout << "eh\n";
  gtk_widget_destroy(dialog);
  std::cout << "file: " << fileToSend << std::endl;
}

bool sendFile(GtkWidget *w, gpointer data)
{
  bool res = scp::sendFile(
    (std::string)gtk_entry_get_text(GTK_ENTRY(hostNameEntry)),
    (std::string)gtk_entry_get_text(GTK_ENTRY(hostIpEntry)),
    (std::string)gtk_entry_get_text(GTK_ENTRY(fileLocationEntry)),
    ((std::string)gtk_entry_get_text(GTK_ENTRY(hostPasswordEntry)))
    .c_str(),
    1
  );
  if (res == FILE_TRANSFER_SUCCESS)
  {
    std::cout << "file transfer success\n";
    clearAll();
  }
  else
    std::cout << "file transfer fail\n";
  scp::showDialog(res, w);
}

int main(int argc, char *argv[])
{
  gtk_init(&argc, &argv);
  win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  grid = gtk_grid_new();
  title = gtk_label_new("Linux QuickSync");
  chooseFileLabel = gtk_label_new("Choose File");
  fileLocationEntry = gtk_entry_new();
  fileChoosebutton = gtk_button_new_with_label("Browse");
  warning = gtk_label_new("Make sure openssh-server is installed on remote machine");
  hostNameLabel = gtk_label_new("Host name:");
  hostPasswordLabel = gtk_label_new("Host password:");
  hostIpLabel = gtk_label_new("Host IP:");
  hostNameEntry = gtk_entry_new();
  hostPasswordEntry = gtk_entry_new();
  hostIpEntry = gtk_entry_new();
  sendButton = gtk_button_new_with_label("Send");
  clearButton = gtk_button_new_with_label("Clear");
  gtk_widget_set_margin_top(grid, 20);
  gtk_widget_set_margin_left(grid, 20);
  gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
  // gtk_grid_attach(GTK_GRID(grid), title, 1, 0, 10, 1);
  gtk_grid_attach(GTK_GRID(grid), chooseFileLabel, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), fileLocationEntry, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), fileChoosebutton, 2, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), hostIpLabel, 0, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), hostIpEntry, 1, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), hostNameLabel, 0, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), hostNameEntry, 1, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), hostPasswordLabel, 0, 5, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), hostPasswordEntry, 1, 5, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), warning, 0, 6, 10, 1);
  gtk_grid_attach(GTK_GRID(grid), clearButton, 0, 7, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), sendButton, 1, 7, 1, 1);
  g_signal_connect(win, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(fileChoosebutton, "clicked", G_CALLBACK(selectFile), NULL);
  g_signal_connect(sendButton, "clicked", G_CALLBACK(sendFile), NULL);
  g_signal_connect(clearButton, "clicked", G_CALLBACK(clearAll), NULL);
  gtk_container_add(GTK_CONTAINER(win), grid);
  gtk_widget_set_size_request(sendButton, 100, 20);
  gtk_window_set_default_size(GTK_WINDOW(win), 500, 300);
  gtk_window_set_title(GTK_WINDOW(win), "Linux QuickSync");
  gtk_window_set_resizable(GTK_WINDOW(win), false);
  gtk_widget_show_all(win);
  gtk_main();
}