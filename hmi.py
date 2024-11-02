import sys
import serial
from PyQt5.QtWidgets import QApplication, QWidget, QLabel, QLineEdit, QPushButton, QVBoxLayout, QMessageBox

class LoginApp(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        # Create widgets
        self.username_label = QLabel('Username:', self)
        self.username_input = QLineEdit(self)

        self.password_label = QLabel('Password:', self)
        self.password_input = QLineEdit(self)
        self.password_input.setEchoMode(QLineEdit.Password)

        self.login_button = QPushButton('Login', self)
        self.login_button.clicked.connect(self.handle_login)

        # Layout
        layout = QVBoxLayout()
        layout.addWidget(self.username_label)
        layout.addWidget(self.username_input)
        layout.addWidget(self.password_label)
        layout.addWidget(self.password_input)
        layout.addWidget(self.login_button)
        self.setLayout(layout)

        # Window settings
        self.setWindowTitle('Login')
        self.resize(500, 300)

    def handle_login(self):
        # Hardcoded username and password for internal authentication
        valid_username = "admin"
        valid_password = "1234"

        username = self.username_input.text()
        password = self.password_input.text()

        if username == valid_username and password == valid_password:
            self.open_dam_settings()
        else:
            QMessageBox.warning(self, 'Error', 'Invalid username or password.')

    def open_dam_settings(self):
        self.dam_settings = DamSettings()
        self.dam_settings.show()
        self.close()  # Close the login window

class DamSettings(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        # Create widgets
        title_label = QLabel('DAM SETTINGS', self)
        title_label.setStyleSheet("font-size: 20px; font-weight: bold;")
        
        settings_button = QPushButton('Open DAM Manually', self)
        settings_button.clicked.connect(self.send_toggle_command)  # Connect to the new method

        # Layout
        layout = QVBoxLayout()
        layout.addWidget(title_label)
        layout.addWidget(settings_button)
        self.setLayout(layout)

        # Window settings
        self.setWindowTitle('DAM SETTINGS')
        self.resize(600, 400)

    def send_toggle_command(self):
        try:
            # Send the toggle command to ESP32
            with serial.Serial('COM3', 9600, timeout=2) as ser:
                ser.write(b'toggle')  # Send the toggle command
                QMessageBox.information(self, 'Success', 'Toggle command sent!')
        except serial.SerialException as e:
            QMessageBox.critical(self, 'Error', f'Could not connect to COM3: {e}')

if __name__ == '__main__':
    app = QApplication(sys.argv)
    login_app = LoginApp()
    login_app.show()
    sys.exit(app.exec_())
