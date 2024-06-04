#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "findgamedialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QHostAddress>
#include <QNetworkInterface>
#include <boost/asio.hpp>
#include <algorithm>
#include <array>
#include <iostream>

using boost::asio::ip::udp;
using boost::asio::ip::tcp;

MainWindow::MainWindow(QWidget *parent, int port)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , player1Turn(true)
    , gameOver(false)
    , port(port)
    , acceptor(nullptr)
    , socket(nullptr)
    , findGameDialog(new FindGameDialog(this))
    , ioService()  // Inicjalizacja ioService
{
    ui->setupUi(this);

    // Inicjalizacja tablicy buttons
    buttons[0][0] = ui->button00;
    buttons[0][1] = ui->button01;
    buttons[0][2] = ui->button02;
    buttons[1][0] = ui->button10;
    buttons[1][1] = ui->button11;
    buttons[1][2] = ui->button12;
    buttons[2][0] = ui->button20;
    buttons[2][1] = ui->button21;
    buttons[2][2] = ui->button22;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            connect(buttons[i][j], &QPushButton::clicked, [this, i, j]() { handleButtonClick(i, j); });
        }
    }

    connect(ui->hostButton, &QPushButton::clicked, this, &MainWindow::hostGame);
    connect(ui->findButton, &QPushButton::clicked, this, &MainWindow::findGame);
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::connectToServer);

    resetGame();
    ui->hostIpLabel->setVisible(false);  // Ukryj etykietę IP hosta na początku
    ui->serverIpLineEdit->setVisible(false);  // Ukryj pole wprowadzania IP serwera na początku
    ui->connectButton->setVisible(false);  // Ukryj przycisk połączenia na początku
    showGameBoard(false);  // Ukryj planszę gry na początku
}

MainWindow::~MainWindow()
{
    delete ui;
    if (acceptor) delete acceptor;
    if (socket) delete socket;
    if (networkThread.joinable()) networkThread.join();
}

void MainWindow::handleButtonClick(int row, int col)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (gameOver || board[row][col] != ' ' || !player1Turn) return;

    char currentPlayer = player1Turn ? 'X' : 'O';
    playerMove(row, col, currentPlayer);

    updateBoard();
    if (checkWin(currentPlayer) || checkDraw()) {
        gameOver = true;
    }

    player1Turn = !player1Turn;
    updateBoard();

    // Wyślij ruch do drugiego gracza
    if (socket && socket->is_open()) {
        boost::asio::streambuf buf;
        std::ostream os(&buf);
        os << row << ' ' << col << '\n';
        boost::asio::write(*socket, buf);
    }
}

void MainWindow::resetGame()
{
    std::lock_guard<std::mutex> lock(mtx);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j] = ' ';
            buttons[i][j]->setText("");
            buttons[i][j]->setEnabled(true);
        }
    }
    player1Turn = true;
    gameOver = false;
    updateBoard();
}

void MainWindow::updateBoard()
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            buttons[i][j]->setText(board[i][j] == ' ' ? "" : QString(board[i][j]));
            buttons[i][j]->setEnabled(board[i][j] == ' ' && player1Turn);
        }
    }
}

bool MainWindow::checkWin(char player)
{
    for (int i = 0; i < 3; ++i) {
        if (std::all_of(std::begin(board[i]), std::end(board[i]), [player](char c) { return c == player; }) ||
            std::all_of(std::begin(board), std::end(board), [i, player](const std::array<char, 3>& row) { return row[i] == player; }))
            return true;
    }
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player))
        return true;
    return false;
}

bool MainWindow::checkDraw()
{
    for (const auto& row : board) {
        if (std::any_of(std::begin(row), std::end(row), [](char c) { return c == ' '; })) {
            return false;
        }
    }
    return true;
}

void MainWindow::playerMove(int row, int col, char player)
{
    board[row][col] = player;
}

void MainWindow::showGameBoard(bool show)
{
    qDebug() << "Changing game board visibility to" << show;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            buttons[i][j]->setVisible(show);
            qDebug() << "Button [" << i << "][" << j << "] visibility set to" << show;
        }
    }
}

void MainWindow::hostGame()
{
    ui->hostButton->setEnabled(false);
    ui->findButton->setEnabled(false);
    ui->hostIpLabel->setVisible(true);

    // Find and display the first non-loopback IPv4 address
    const auto allAddresses = QNetworkInterface::allAddresses();
    for (const QHostAddress &address : allAddresses) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress::LocalHost) {
            ui->hostIpLabel->setText("Host IP: " + address.toString());
            break;
        }
    }

    ui->serverIpLineEdit->setVisible(false);
    ui->connectButton->setVisible(false);

    networkThread = std::thread([this]() { startServer(); });
}

void MainWindow::findGame()
{
    ui->hostButton->setEnabled(false);
    ui->findButton->setEnabled(false);
    ui->serverIpLineEdit->setVisible(true);
    ui->connectButton->setVisible(true);

    networkThread = std::thread([this]() { broadcastFindGame(); });
}

void MainWindow::startServer()
{
    try {
        boost::asio::io_service ioService;
        tcp::acceptor acceptor(ioService, tcp::endpoint(tcp::v4(), port));
        tcp::socket socket(ioService);
        acceptor.accept(socket);

        this->socket = new tcp::socket(std::move(socket));
        
        // Wywołanie showGameBoard, aby wyświetlić planszę
        QMetaObject::invokeMethod(this, "showGameBoard", Qt::QueuedConnection, Q_ARG(bool, true));

        while (true) {
            boost::asio::streambuf buf;
            boost::asio::read_until(*this->socket, buf, '\n');
            std::istream is(&buf);
            int row, col;
            is >> row >> col;
            handleButtonClick(row, col);
        }
    } catch (std::exception& e) {
        std::cerr << "Exception in startServer: " << e.what() << std::endl;
    }
}

void MainWindow::connectToServer()
{
    qDebug() << "Entering connectToServer...";

    QString hostIp = ui->serverIpLineEdit->text();
    qDebug() << "Host IP:" << hostIp;

    ui->serverIpLineEdit->setEnabled(false);
    ui->connectButton->setEnabled(false);

    try {
        qDebug() << "Connecting to server directly...";
        startClient(hostIp.toStdString());
    } catch (const std::exception &e) {
        // If an error occurs, display a message box
        qCritical() << "Exception in connectToServer:" << e.what();
        ui->serverIpLineEdit->setEnabled(true);
        ui->connectButton->setEnabled(true);
        QMessageBox::critical(this, "Connection Error", e.what());
    } catch (...) {
        qCritical() << "Unknown exception in connectToServer";
        ui->serverIpLineEdit->setEnabled(true);
        ui->connectButton->setEnabled(true);
        QMessageBox::critical(this, "Connection Error", "An unknown error occurred.");
    }
}

void MainWindow::startClient(const std::string &host)
{
    qDebug() << "Entering startClient with host:" << QString::fromStdString(host);

    try {
        qDebug() << "Creating io_service...";
        boost::asio::io_service ioService;

        qDebug() << "Creating resolver...";
        tcp::resolver resolver(ioService);

        qDebug() << "Creating query...";
        tcp::resolver::query query(host, std::to_string(port));

        qDebug() << "Resolving query...";
        tcp::resolver::iterator endpointIterator = resolver.resolve(query);
        qDebug() << "Endpoint resolved.";

        qDebug() << "Creating socket...";
        tcp::socket socket(ioService);

        qDebug() << "Connecting socket...";
        boost::asio::connect(socket, endpointIterator);
        qDebug() << "Socket connected.";

        qDebug() << "Moving socket...";
        this->socket = new tcp::socket(std::move(socket));

        qDebug() << "Showing game board...";
        QMetaObject::invokeMethod(this, "showGameBoard", Qt::QueuedConnection, Q_ARG(bool, true));

        qDebug() << "Client connected and entering main loop...";

        while (true) {
            qDebug() << "Waiting for data...";
            boost::asio::streambuf buf;
            boost::asio::read_until(*this->socket, buf, '\n');
            qDebug() << "Data received";

            std::istream is(&buf);
            int row, col;
            is >> row >> col;

            qDebug() << "Received move: row =" << row << ", col =" << col;

            if (is) {
                QMetaObject::invokeMethod(this, [this, row, col]() {
                    handleButtonClick(row, col);
                }, Qt::QueuedConnection);
            } else {
                qCritical() << "Failed to parse move data.";
                break;
            }
        }
    } catch (const boost::system::system_error &e) {
        qCritical() << "Boost system error in startClient:" << e.what();
        throw std::runtime_error("Failed to connect to the server: " + std::string(e.what()));
    } catch (const std::exception &e) {
        qCritical() << "Standard exception in startClient:" << e.what();
        throw std::runtime_error("Failed to connect to the server: " + std::string(e.what()));
    } catch (...) {
        qCritical() << "Unknown exception in startClient";
        throw std::runtime_error("Failed to connect to the server: Unknown error");
    }
}

void MainWindow::broadcastFindGame()
{
    try {
        udp::socket socket(ioService, udp::endpoint(udp::v4(), 0));
        socket.set_option(boost::asio::socket_base::broadcast(true));

        udp::endpoint receiverEndpoint(boost::asio::ip::address_v4::broadcast(), port);

        std::string request = "FIND_GAME";
        socket.send_to(boost::asio::buffer(request), receiverEndpoint);

        char reply[1024];
        udp::endpoint senderEndpoint;
        size_t len = socket.receive_from(boost::asio::buffer(reply, 1024), senderEndpoint);

        if (std::string(reply).substr(0, len) == "GAME_HOST") {
            ui->serverIpLineEdit->setText(senderEndpoint.address().to_string().c_str());
        }
    } catch (std::exception& e) {
        std::cerr << "Exception in broadcastFindGame: " << e.what() << std::endl;
    }
}

void MainWindow::showHostIp()
{
    ui->hostIpLabel->setText("Host IP: " + QHostAddress(QNetworkInterface::allAddresses().at(1)).toString());
}

void MainWindow::connectToHost()
{
    QString hostIp = ui->serverIpLineEdit->text();
    startClient(hostIp.toStdString());
}

