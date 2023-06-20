import subprocess
import time
from http.server import BaseHTTPRequestHandler, HTTPServer

# Сброс состояний пинов
subprocess.run(['gpio', 'reset'])

# Устанавливаем режим работы пинов на выход
subprocess.run(['gpio', '-1', 'mode', '7', 'out'])
subprocess.run(['gpio', '-1', 'mode', '11', 'out'])
subprocess.run(['gpio', '-1', 'mode', '13', 'out'])
subprocess.run(['gpio', '-1', 'mode', '15', 'out'])

# Автотестирование выходов
print('Идет тестирование выходов, подождите...')
subprocess.run(['gpio',  '-1', 'write', '11', '0'])
time.sleep(1)
subprocess.run(['gpio',  '-1', 'write', '7', '0'])
time.sleep(1)
subprocess.run(['gpio',  '-1', 'write', '15', '0'])
time.sleep(1)
subprocess.run(['gpio',  '-1', 'write', '13', '0'])
time.sleep(1)
subprocess.run(['gpio',  '-1', 'write', '11', '1'])
subprocess.run(['gpio',  '-1', 'write', '7', '1'])
subprocess.run(['gpio',  '-1', 'write', '15', '1'])
subprocess.run(['gpio',  '-1', 'write', '13', '1'])
print('Тестирование окончено. Готов к работе!')


class RequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/3':
            subprocess.run(['gpio',  '-1', 'write', '11', '0'])
            subprocess.run(['gpio',  '-1', 'write', '7', '1'])
            subprocess.run(['gpio',  '-1', 'write', '15', '1'])
            subprocess.run(['gpio',  '-1', 'write', '13', '1'])
            self.send_response(200)
            self.end_headers()
        elif self.path == '/4':
            subprocess.run(['gpio',  '-1', 'write', '11', '1'])
            subprocess.run(['gpio',  '-1', 'write', '7', '0'])
            subprocess.run(['gpio',  '-1', 'write', '15', '1'])
            subprocess.run(['gpio',  '-1', 'write', '13', '1'])
            self.send_response(200)
            self.end_headers()
        elif self.path == '/5':
            subprocess.run(['gpio',  '-1', 'write', '11', '1'])
            subprocess.run(['gpio',  '-1', 'write', '7', '1'])
            subprocess.run(['gpio',  '-1', 'write', '15', '0'])
            subprocess.run(['gpio',  '-1', 'write', '13', '1'])
            self.send_response(200)
            self.end_headers()
        elif self.path == '/1':
            subprocess.run(['gpio',  '-1', 'write', '11', '1'])
            subprocess.run(['gpio',  '-1', 'write', '7', '1'])
            subprocess.run(['gpio',  '-1', 'write', '15', '1'])
            subprocess.run(['gpio',  '-1', 'write', '13', '0'])
            self.send_response(200)
            self.end_headers()
        elif self.path == '/0':
            subprocess.run(['gpio',  '-1', 'write', '11', '1'])
            subprocess.run(['gpio',  '-1', 'write', '7', '1'])
            subprocess.run(['gpio',  '-1', 'write', '15', '1'])
            subprocess.run(['gpio',  '-1', 'write', '13', '1'])
            self.send_response(200)
            self.end_headers()
        else:
            self.send_response(404)
            self.end_headers()

def run_server(port=8000):
    server_address = ('', port)
    httpd = HTTPServer(server_address, RequestHandler)
    print('Сервер запущен на порту', port)
    httpd.serve_forever()

if __name__ == '__main__':
    run_server()
