extends Node

# Constants
const SLOT_SIZE = Vector2(66, 66)

const ACTION_MODE_NONE = 0
const ACTION_MODE_MOVE = 1
const ACTION_MODE_ATTACK = 2
const ACTION_MODE_ACTIVATION = 3
const ACTION_MODE_WAIT = 4
const ACTION_MODE_SPEAK = 5
const ACTION_MODE_SLEEP = 6
const ACTION_MODE_SKILL = 7

# Colors
const ink = Color("2f2f2f")
const white = Color("c3c3c3")
const gold = Color("ffcc00")
const transparent_gold = Color("ffcc003f")

# Variables
var server_owner = false
var ip = "127.0.0.1"
var multiplayer_mode = true
var link = GodotLink.new()
var macros = {}
var free_mouse_state = false
var tickrate
var updating_state = false
var next_state_ready = false
#var mode = ACTION_MODE_NONE

var coord = Vector3.ZERO

var action = {}
var move_set = false
var action_mutex = Mutex.new()
