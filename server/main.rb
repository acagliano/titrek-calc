#!/usr/bin/ruby -w
require 'socket'


class MapObject
    @@map_count
    def initialize(type, mass, radius, gravity)
        @object_id = id
        @object_mass = mass
        @object_radius = radius
        @object_gravity = gravity
        @@map_count+=1;
    end
    @x_coord
    @y_coord
    @z_coord
end

class Entity
    @@entity_count
    def initialize(type

BEGIN {
    puts "Initializing StarTrek MP Server..."
    # Let's initialize the map
    
    PORT =
    socket = socket = TCPServer.new('0.0.0.0', PORT)
}

def conn_recv(client)
    client.close
end


loop do
    players[]
    client = socket.accept
    players << Thread.new { conn_recv(client) }
end
