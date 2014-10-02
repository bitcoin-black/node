#include <gtest/gtest.h>

#include <mu_coin_qt/qt.hpp>
#include <thread>
#include <QTest>

TEST (client, construction)
{
    mu_coin::system system (1, 24000, 25000, 1);
    int argc (0);
    QApplication application (argc, nullptr);
    mu_coin_qt::client client (application, *system.clients [0]);
}

TEST (client, main)
{
    mu_coin::system system (1, 24000, 25000, 1);
    int argc (0);
    QApplication application (argc, nullptr);
    mu_coin_qt::client client (application, *system.clients [0]);
    ASSERT_EQ (client.entry_window, client.main_stack->currentWidget ());
    QTest::mouseClick (client.send_coins, Qt::LeftButton);
    ASSERT_EQ (client.send_coins_window, client.main_stack->currentWidget ());
    QTest::mouseClick (client.send_coins_back, Qt::LeftButton);
    ASSERT_EQ (client.entry_window, client.main_stack->currentWidget ());
    QTest::mouseClick (client.show_wallet, Qt::LeftButton);
    ASSERT_EQ (client.wallet_window, client.main_stack->currentWidget ());
    QTest::mouseClick (client.wallet_back, Qt::LeftButton);
    ASSERT_EQ (client.entry_window, client.main_stack->currentWidget ());
    QTest::mouseClick (client.settings, Qt::LeftButton);
    ASSERT_EQ (client.settings_window, client.main_stack->currentWidget ());
    QTest::mouseClick (client.settings_change_password_button, Qt::LeftButton);
    ASSERT_EQ (client.password_change.window, client.main_stack->currentWidget ());
    QTest::mouseClick (client.password_change.back, Qt::LeftButton);
    ASSERT_EQ (client.settings_window, client.main_stack->currentWidget ());
    QTest::mouseClick (client.settings_back, Qt::LeftButton);
    ASSERT_EQ (client.entry_window, client.main_stack->currentWidget ());
    QTest::mouseClick (client.show_ledger, Qt::LeftButton);
    ASSERT_EQ (client.ledger_window, client.main_stack->currentWidget ());
    QTest::mouseClick (client.ledger_back, Qt::LeftButton);
    ASSERT_EQ (client.entry_window, client.main_stack->currentWidget ());
    QTest::mouseClick (client.show_peers, Qt::LeftButton);
    ASSERT_EQ (client.peers_window, client.main_stack->currentWidget ());
    QTest::mouseClick (client.peers_back, Qt::LeftButton);
    ASSERT_EQ (client.entry_window, client.main_stack->currentWidget ());
    QTest::mouseClick (client.show_log, Qt::LeftButton);
    ASSERT_EQ (client.log_window, client.main_stack->currentWidget ());
    QTest::mouseClick (client.log_back, Qt::LeftButton);
    ASSERT_EQ (client.entry_window, client.main_stack->currentWidget ());
}

TEST (client, password_change)
{
    mu_coin::system system (1, 24000, 25000, 1);
    int argc (0);
    QApplication application (argc, nullptr);
    mu_coin_qt::client client (application, *system.clients [0]);
    QTest::mouseClick (client.settings, Qt::LeftButton);
    QTest::mouseClick (client.settings_change_password_button, Qt::LeftButton);
    ASSERT_NE (client.client_m.wallet.hash_password ("1"), client.client_m.wallet.password);
    QTest::keyClicks (client.password_change.password, "1");
    QTest::keyClicks (client.password_change.retype, "1");
    QTest::mouseClick (client.password_change.change, Qt::LeftButton);
    ASSERT_EQ (client.client_m.wallet.hash_password ("1"), client.client_m.wallet.password);
    ASSERT_EQ ("", client.password_change.password->text ());
    ASSERT_EQ ("", client.password_change.retype->text ());
}

TEST (client, password_nochange)
{
    mu_coin::system system (1, 24000, 25000, 1);
    int argc (0);
    QApplication application (argc, nullptr);
    mu_coin_qt::client client (application, *system.clients [0]);
    QTest::mouseClick (client.settings, Qt::LeftButton);
    QTest::mouseClick (client.settings_change_password_button, Qt::LeftButton);
    ASSERT_EQ (client.client_m.wallet.hash_password (""), client.client_m.wallet.password);
    QTest::keyClicks (client.password_change.password, "1");
    QTest::keyClicks (client.password_change.retype, "2");
    QTest::mouseClick (client.password_change.change, Qt::LeftButton);
    ASSERT_EQ (client.client_m.wallet.hash_password (""), client.client_m.wallet.password);
    ASSERT_EQ ("1", client.password_change.password->text ());
    ASSERT_EQ ("2", client.password_change.retype->text ());
}

TEST (client, enter_password)
{
    mu_coin::system system (1, 24000, 25000, 1);
    int argc (0);
    QApplication application (argc, nullptr);
    mu_coin_qt::client client (application, *system.clients [0]);
    ASSERT_NE (-1, client.enter_password.layout->indexOf (client.enter_password.valid));
    ASSERT_NE (-1, client.enter_password.layout->indexOf (client.enter_password.password));
    ASSERT_NE (-1, client.enter_password.layout->indexOf (client.enter_password.unlock));
    ASSERT_NE (-1, client.enter_password.layout->indexOf (client.enter_password.lock));
    ASSERT_NE (-1, client.enter_password.layout->indexOf (client.enter_password.back));
    ASSERT_FALSE (client.client_m.wallet.rekey (client.client_m.wallet.hash_password("abc")));
    QTest::mouseClick (client.settings, Qt::LeftButton);
    QTest::mouseClick (client.settings_enter_password_button, Qt::LeftButton);
    QTest::keyClicks (client.enter_password.password, "a");
    QTest::mouseClick (client.enter_password.unlock, Qt::LeftButton);
    ASSERT_EQ ("Password: INVALID", client.enter_password.valid->text ());
    client.enter_password.password->setText ("");
    QTest::keyClicks (client.enter_password.password, "abc");
    QTest::mouseClick (client.enter_password.unlock, Qt::LeftButton);
    ASSERT_EQ ("Password: Valid", client.enter_password.valid->text ());
    ASSERT_EQ ("", client.enter_password.password->text ());
}

TEST (client, send)
{
    mu_coin::system system (1, 24000, 25000, 2);
    system.clients [0]->wallet.insert (mu_coin::test_genesis_key.prv);
    mu_coin::keypair key1;
    std::string account;
    key1.pub.encode_base58check (account);
    system.clients [1]->wallet.insert (key1.prv);
    int argc (0);
    QApplication application (argc, nullptr);
    mu_coin_qt::client client (application, *system.clients [0]);
    QTest::mouseClick (client.send_coins, Qt::LeftButton);
    QTest::keyClicks (client.send_address, account.c_str ());
    QTest::keyClicks (client.send_count, "2");
    QTest::mouseClick (client.send_coins_send, Qt::LeftButton);
    while (client.client_m.ledger.account_balance (key1.pub).is_zero ())
    {
        system.service->poll_one ();
        system.processor.poll_one ();
    }
	ASSERT_EQ (2 * client.client_m.scale, client.client_m.ledger.account_balance (key1.pub));
	QTest::mouseClick (client.send_coins_back, Qt::LeftButton);
	QTest::mouseClick (client.show_ledger, Qt::LeftButton);
	QTest::mouseClick (client.ledger_refresh, Qt::LeftButton);
	ASSERT_EQ (2, client.ledger_model->rowCount ());
	ASSERT_EQ (3, client.ledger_model->columnCount ());
	auto item (client.ledger_model->itemFromIndex (client.ledger_model->index (1, 1)));
	ASSERT_EQ ("2", item->text ().toStdString ());
}