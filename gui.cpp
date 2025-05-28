#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
#include <algorithm>
#include "Game.hpp"
#include "Player.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"

class GameGUI : public QWidget
{
private:
    Game game;
    QTextEdit *gameInfo;
    QTextEdit *currentPlayerInfo;
    QVBoxLayout *actionLayout;
    QPushButton *addPlayerBtn;
    QPushButton *startGameBtn;
    bool gameStarted = false;

public:
    GameGUI(QWidget *parent = nullptr) : QWidget(parent)
    {
        setupUI();
    }

    void setupUI()
    {
        setWindowTitle("Coup Game");
        setMinimumSize(800, 600);

        QHBoxLayout *mainLayout = new QHBoxLayout(this);

        // Left panel - Game info
        QVBoxLayout *leftPanel = new QVBoxLayout();
        gameInfo = new QTextEdit();
        gameInfo->setReadOnly(true);
        gameInfo->setMaximumWidth(300);
        leftPanel->addWidget(new QLabel("Game Status:"));
        leftPanel->addWidget(gameInfo);

        // Right panel - Player actions
        QVBoxLayout *rightPanel = new QVBoxLayout();

        // Current player info
        currentPlayerInfo = new QTextEdit();
        currentPlayerInfo->setReadOnly(true);
        currentPlayerInfo->setMaximumHeight(100);
        rightPanel->addWidget(new QLabel("Current Player:"));
        rightPanel->addWidget(currentPlayerInfo);

        // Game setup buttons
        QHBoxLayout *setupLayout = new QHBoxLayout();
        addPlayerBtn = new QPushButton("Add Player");
        startGameBtn = new QPushButton("Start Game");
        setupLayout->addWidget(addPlayerBtn);
        setupLayout->addWidget(startGameBtn);
        rightPanel->addLayout(setupLayout);

        // Action buttons area
        actionLayout = new QVBoxLayout();
        rightPanel->addWidget(new QLabel("Actions:"));
        rightPanel->addLayout(actionLayout);

        mainLayout->addLayout(leftPanel);
        mainLayout->addLayout(rightPanel);

        // Connect buttons
        connect(addPlayerBtn, &QPushButton::clicked, this, &GameGUI::addPlayer);
        connect(startGameBtn, &QPushButton::clicked, this, &GameGUI::startGame);

        updateDisplay();
    }

    void addPlayer()
    {
        bool ok;
        QString name = QInputDialog::getText(this, "Add Player", "Player name:", QLineEdit::Normal, "", &ok);
        if (ok && !name.isEmpty())
        {
            try
            {
                auto player = std::make_shared<Player>(name.toStdString());
                game.addPlayer(player);
                updateDisplay();
            }
            catch (const std::exception &e)
            {
                QMessageBox::warning(this, "Error", e.what());
            }
        }
    }

    void startGame()
    {
        if (game.getPlayers().size() < 2)
        {
            QMessageBox::warning(this, "Error", "Need at least 2 players to start");
            return;
        }
        gameStarted = true;
        addPlayerBtn->setEnabled(false);
        startGameBtn->setEnabled(false);
        updateDisplay();
        updateActionButtons();
    }

    void updateDisplay()
    {
        // Update game info
        QString info = QString("Players: %1\nPool: %2 coins\n").arg(game.getPlayers().size()).arg(game.getCoinPool());

        if (gameStarted)
        {
            info += QString("Current turn: %1\n").arg(QString::fromStdString(game.currentPlayerTurn()->getName()));
        }

        info += "\nPlayers status:\n";
        for (const auto &player : game.getPlayers())
        {
            if (player->isAlive())
            {
                info += QString("%1 (%2): %3 coins")
                            .arg(QString::fromStdString(player->getName()))
                            .arg(QString::fromStdString(player->getRole()->name()))
                            .arg(player->getCoins());

                if (player->getisSanctioned())
                    info += " [SANCTIONED]";
                if (player->isArrestBlocked())
                    info += " [ARREST BLOCKED]";
                if (player->getMustCoup())
                    info += " [MUST COUP]";
                info += "\n";
            }
        }

        gameInfo->setText(info);

        // Update current player info
        if (gameStarted && !game.getPlayers().empty())
        {
            auto currentPlayer = game.currentPlayerTurn();
            QString playerInfo = QString("%1 (%2)\nCoins: %3")
                                     .arg(QString::fromStdString(currentPlayer->getName()))
                                     .arg(QString::fromStdString(currentPlayer->getRole()->name()))
                                     .arg(currentPlayer->getCoins());

            if (currentPlayer->getisSanctioned())
            {
                playerInfo += "\n SANCTIONED - Cannot gather/tax!";
            }
            currentPlayerInfo->setText(playerInfo);
        }
    }

    void updateActionButtons()
    {
        // Clear existing buttons
        QLayoutItem *item;
        while ((item = actionLayout->takeAt(0)) != nullptr)
        {
            delete item->widget();
            delete item;
        }

        if (!gameStarted)
            return;

        auto currentPlayer = game.currentPlayerTurn();

        // Basic actions
        QPushButton *gatherBtn = new QPushButton("Gather");
        QPushButton *taxBtn = new QPushButton("Tax");
        QPushButton *bribeBtn = new QPushButton("Bribe");
        QPushButton *arrestBtn = new QPushButton("Arrest");
        QPushButton *sanctionBtn = new QPushButton("Sanction");
        QPushButton *coupBtn = new QPushButton("Coup");

        actionLayout->addWidget(gatherBtn);
        actionLayout->addWidget(taxBtn);
        actionLayout->addWidget(bribeBtn);
        actionLayout->addWidget(arrestBtn);
        actionLayout->addWidget(sanctionBtn);
        actionLayout->addWidget(coupBtn);

        // Actions requiring targets
        if (currentPlayer->getCoins() >= 4)
        {
            connect(bribeBtn, &QPushButton::clicked, [this]()
                    { performBribe(); });
        }

        if (currentPlayer->getCoins() >= 3)
        {
            connect(sanctionBtn, &QPushButton::clicked, [this]()
                    { performSanction(); });
        }
        if (currentPlayer->getCoins() >= 10 || currentPlayer->getMustCoup())
        {
            QLabel* warningLabel = new QLabel(" YOU MUST COUP - 10+ COINS!");
        warningLabel->setStyleSheet("color: red; font-weight: bold;");
        actionLayout->addWidget(warningLabel);
         connect(coupBtn, &QPushButton::clicked, [this]() { performCoup(); });
        return;
        }

        // Role-specific actions
        if (currentPlayer->getRole()->name() == "Baron" && currentPlayer->getCoins() >= 3)
        {
            QPushButton *investBtn = new QPushButton("Baron: Invest (3→6 coins)");
            actionLayout->addWidget(investBtn);
            connect(investBtn, &QPushButton::clicked, [this]()
                    { performInvest(); });
        }

        if (currentPlayer->getRole()->name() == "Spy")
        {
            QPushButton *spyBtn = new QPushButton("Spy: See coins/Block arrest");
            actionLayout->addWidget(spyBtn);
            connect(spyBtn, &QPushButton::clicked, [this]()
                    { performSpyAction(); });
        }

        if (currentPlayer->getRole()->name() == "General" && currentPlayer->getCoins() >= 5)
        {
            QPushButton *preventBtn = new QPushButton("General: Prevent Coup (5 coins)");
            actionLayout->addWidget(preventBtn);
            connect(preventBtn, &QPushButton::clicked, [this]()
                    { performPreventCoup(); });
        }
        if (currentPlayer->getRole()->name() == "Judge")
        {
            QPushButton *cancelBribeBtn = new QPushButton("Judge: Cancel Bribe");
            actionLayout->addWidget(cancelBribeBtn);
            connect(cancelBribeBtn, &QPushButton::clicked, [this]()
                    { performCancelBribe(); });
        }
        if (currentPlayer->getRole()->name() == "Governor")
        {
            QPushButton *cancelTaxBtn = new QPushButton("Governor: Cancel Tax");
            actionLayout->addWidget(cancelTaxBtn);
            connect(cancelTaxBtn, &QPushButton::clicked, [this]()
                    { performCancelTax(); });
        }

        // Connect basic actions
        connect(gatherBtn, &QPushButton::clicked, [this]()
                { performGather(); });
        connect(taxBtn, &QPushButton::clicked, [this]()
                { performTax(); });
        connect(arrestBtn, &QPushButton::clicked, [this]()
                { performArrest(); });
        connect(coupBtn, &QPushButton::clicked, [this]()
                { performCoup(); });
    }

    void performGather()
    {
        try
        {
            game.currentPlayerTurn()->gatherAction(game);
            updateDisplay();
            updateActionButtons();
        }
        catch (const std::exception &e)
        {
            QMessageBox::warning(this, "Error", e.what());
        }
    }

    void performTax()
    {
        try
        {
            game.currentPlayerTurn()->taxAction(game);
            updateDisplay();
            updateActionButtons();
        }
        catch (const std::exception &e)
        {
            QMessageBox::warning(this, "Error", e.what());
        }
    }

    void performBribe()
    {
        try
        {
            game.currentPlayerTurn()->bribeAction(game);
            updateDisplay();
            updateActionButtons();
        }
        catch (const std::exception &e)
        {
            QMessageBox::warning(this, "Error", e.what());
        }
    }

    void performSanction()
    {
        auto target = selectTarget("Select target to sanction:");
        if (target)
        {
            try
            {
                game.currentPlayerTurn()->sanctionAction(target, game);
                updateDisplay();
                updateActionButtons();
            }
            catch (const std::exception &e)
            {
                QMessageBox::warning(this, "Error", e.what());
            }
        }
    }

    void performArrest()
    {
        auto target = selectTarget("Select target to arrest:");
        if (target)
        {
            try
            {
                game.currentPlayerTurn()->arrestAction(target, game);
                updateDisplay();
                updateActionButtons();
            }
            catch (const std::exception &e)
            {
                QMessageBox::warning(this, "Error", e.what());
            }
        }
    }

    void performCoup()
    {
        auto target = selectTarget("Select target to coup:");
        if (target)
        {
            try
            {
                game.currentPlayerTurn()->coupAction(target, game);
                updateDisplay();
                updateActionButtons();

                // Check for winner
                // Count alive players
                int alivePlayers = 0;
                std::shared_ptr<Player> lastAlivePlayer = nullptr;
                for (const auto &player : game.getPlayers())
                {
                    if (player->isAlive())
                    {
                        alivePlayers++;
                        lastAlivePlayer = player;
                    }
                }

                if (alivePlayers == 1 && lastAlivePlayer)
                {
                    QMessageBox::information(this, "Game Over",
                                             QString("Winner: %1").arg(QString::fromStdString(lastAlivePlayer->getName())));
                }
            }
            catch (const std::exception &e)
            {
                QMessageBox::warning(this, "Error", e.what());
            }
        }
    }

    void performInvest()
    {
        try
        {
            game.currentPlayerTurn()->investAction(game);
            updateDisplay();
            updateActionButtons();
        }
        catch (const std::exception &e)
        {
            QMessageBox::warning(this, "Error", e.what());
        }
    }

    void performSpyAction()
    {
        auto target = selectTarget("Select target for spy action:");
        if (target)
        {
            try
            {
                auto spy = std::dynamic_pointer_cast<Spy>(game.currentPlayerTurn()->getRole());
                if (spy)
                {
                    spy->blockArrest(target);
                    spy->seeTargetCoins(*game.currentPlayerTurn(), target);
                    
                    QMessageBox::information(this, "Spy Action",
                                             QString("%1 has %2 coins and arrest is blocked next turn")
                                                 .arg(QString::fromStdString(target->getName()))
                                                 .arg(target->getCoins()));
                }
                updateDisplay();
                updateActionButtons();
            }
            catch (const std::exception &e)
            {
                QMessageBox::warning(this, "Error", e.what());
            }
        }
    }

    void performPreventCoup()
    {
        auto target = selectTarget("Select target to protect from coup:");
        if (target)
        {
            try
            {
                auto general = std::dynamic_pointer_cast<General>(game.currentPlayerTurn()->getRole());
                if (general)
                {
                    general->preventCoup(*game.currentPlayerTurn(), target, game);
                }
                updateDisplay();
                updateActionButtons();
            }
            catch (const std::exception &e)
            {
                QMessageBox::warning(this, "Error", e.what());
            }
        }
    }
    void performCancelTax()
    {
        auto currentPlayer = game.currentPlayerTurn();

        auto target = selectTarget("Select target whose tax to cancel:");
        if (target)
        {
            try
            {
                auto governor = std::dynamic_pointer_cast<Governor>(currentPlayer->getRole());
                if (governor)
                {
                    governor->cancelTax(*currentPlayer, target, game);
                    updateDisplay();
                    updateActionButtons();
                    QMessageBox::information(this, "Tax Cancelled",
                                             QString("Governor cancelled %1's tax action!")
                                                 .arg(QString::fromStdString(target->getName())));
                }
            }
            catch (const std::exception &e)
            {
                QMessageBox::warning(this, "Cannot Cancel Tax", e.what());
            }
        }
    }

    void performCancelBribe()
    {
        auto currentPlayer = game.currentPlayerTurn();

        auto target = selectTarget("Select target whose bribe to cancel:");
        if (target)
        {
            try
            {
                auto judge = std::dynamic_pointer_cast<Judge>(currentPlayer->getRole());
                if (judge)
                {
                    judge->cancelBribe(*currentPlayer, target, game);
                    updateDisplay();
                    updateActionButtons();
                    QMessageBox::information(this, "Bribe Cancelled",
                                             QString("Judge cancelled %1's bribe action!")
                                                 .arg(QString::fromStdString(target->getName())));
                }
            }
            catch (const std::exception &e)
            {
                QMessageBox::warning(this, "Cannot Cancel Bribe", e.what());
            }
        }
    }

    std::shared_ptr<Player> selectTarget(const QString &title)
    {
        QStringList playerNames;
        std::vector<std::shared_ptr<Player>> availablePlayers;

        for (const auto &player : game.getPlayers())
        {
            if (player->isAlive() && player != game.currentPlayerTurn())
            {
                playerNames << QString::fromStdString(player->getName());
                availablePlayers.push_back(player);
            }
        }

        if (playerNames.isEmpty())
        {
            QMessageBox::warning(this, "Error", "No valid targets available");
            return nullptr;
        }

        bool ok;
        QString selected = QInputDialog::getItem(this, title, "Select player:", playerNames, 0, false, &ok);

        if (ok)
        {
            int index = playerNames.indexOf(selected);
            return availablePlayers[index];
        }

        return nullptr;
    }

    void nextTurn()
    {
        game.nextTurn();
        updateDisplay();
        updateActionButtons();
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    GameGUI window;
    window.show();

    return app.exec();
}