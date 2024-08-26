#include <iostream>
#define FILE_TRANSFER_SUCCESS 1
#define FILE_TRANSFER_FAIL 0

class scp
{
public:
  static bool sshpassIsInstalled()
  {
    FILE *sshcheck = popen("dpkg -s sshpass >/dev/null 2>&1 && echo \"Installed\" || echo \"Not installed\"", "r");
    if (!sshcheck)
      std::cerr << "unknown error occured\n";
    char buffer[128];
    std::string output;
    while (fgets(buffer, sizeof(buffer), sshcheck) != NULL)
      output += buffer;
    if (output == "Installed\n")
      return 1;
    return 0;
  }
  static bool sendFile(std::string remoteHost, std::string remoteIp, std::string fileName, const char *password, bool isFile)
  {
    if (!sshpassIsInstalled())
    {
      std::cout << "installing sshpass...\n";
      FILE *sshInstall = popen("sudo apt install -y sshpass", "w");
      if (!sshInstall)
        std::cerr << "unknown error occured while installing...\n";
      fprintf(sshInstall, "%s\n", password);
      fclose(sshInstall);
    }
    std::string scpCommand = "sshpass -p " + (std::string)password + " scp -o StrictHostKeyChecking=no " + (isFile ? "" : "-r ") + fileName + " " + remoteHost + "@" + remoteIp + ":/home/" + remoteHost + "/Documents 2>&1 && echo success";
    FILE *pipe = popen(scpCommand.c_str(), "r");
    if (!pipe)
      std::cerr << "popen() failed\n";
    std::string output = "";
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL)
      output += buffer;
    pclose(pipe);
    std::cout << "output " << output << "\n";
    if (output.find("success") != std::string::npos)
      return FILE_TRANSFER_SUCCESS;
    else
      return FILE_TRANSFER_FAIL;
    // return output;
  }
  static void showDialog(int fileTransferStatus, GtkWidget *parentWindow)
  {
    GtkWidget *dialog, *message, *contentArea;
    dialog = gtk_dialog_new_with_buttons(
      "Transfer Status",
      (GtkWindow *)parentWindow,
      GTK_DIALOG_MODAL,
      GTK_STOCK_OK,
      GTK_RESPONSE_OK
    );
    contentArea = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    if (fileTransferStatus == FILE_TRANSFER_SUCCESS)
      message = gtk_label_new("File sent successfully");
    else
      message = gtk_label_new("File Transfer Failed");
    gtk_container_add(GTK_CONTAINER(contentArea), message);
    // gtk_container_add(GTK_CONTAINER(dialog), message);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 200, 200);
    gtk_widget_show_all(dialog);
    gint res = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
  }
};