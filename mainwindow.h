#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <array>
#include <thread>
#include <mutex>
#include <boost/asio.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class FindGameDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, int port = 12345);
    ~MainWindow();

private slots:
    void handleButtonClick(int row, int col);
    void hostGame();
    void findGame();
    void connectToServer();
    void connectToHost();

private:
    Ui::MainWindow *ui;
    FindGameDialog *findGameDialog;
    std::array<std::array<QPushButton*, 3>, 3> buttons;
    std::array<std::array<char, 3>, 3> board;
    bool player1Turn;
    bool gameOver;
    int port;
    boost::asio::ip::tcp::acceptor *acceptor;
    boost::asio::ip::tcp::socket *socket;
    boost::asio::io_service ioService;
    std::thread networkThread;
    std::mutex mtx;

    void resetGame();
    void updateBoard();
    bool checkWin(char player);
    bool checkDraw();
    void playerMove(int row, int col, char player);
    void showGameBoard(bool show);
    void startServer();
    void startClient(const std::string &host);
    void broadcastFindGame();
    void showHostIp();
};

#endif // MAINWINDOW_H

