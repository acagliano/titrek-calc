
require 'socket'

PORT =
socket = socket = TCPServer.new('0.0.0.0', PORT)

def conn_recv(client)

end


loop do
    players[]
    client = socket.accept
    players << Thread.new { conn_recv(client) }
end
