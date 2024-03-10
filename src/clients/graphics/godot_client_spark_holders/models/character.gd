extends CharacterBody3D

var character
var id

# How fast the player moves in meters per second.
@export var speed = 8 #14
@export var accel = 40
# The downward acceleration when in the air, in meters per second squared.
@export var fall_acceleration = 40 #75

var target_velocity = Vector3.ZERO

@onready var nav = $NavigationAgent3D
@onready var range_mesh = $AttackRange
@onready var shape = $Shape

func set_color(color: String):
	var material = StandardMaterial3D.new()
	material.albedo_color = color
	$Color.set_surface_override_material(0, material)
	$Selection.set_surface_override_material(0, material)

func set_attack_range(weapon_range: float):
	$AttackRange.scale_object_local(Vector3(weapon_range, weapon_range, weapon_range))

func select():
	$Selection.visible = true

func unselect():
	$Selection.visible = false

func move_towards(dest: Vector3, delta: float):
	nav.target_position = dest
	var checkpoint = nav.get_next_path_position()
	checkpoint.y = transform.origin.y
	var direction = (checkpoint - transform.origin).normalized()
	velocity = velocity.lerp(direction * speed, accel * delta)
	#if not is_on_floor(): # If in the air, fall towards the floor. Literally gravity
	#	velocity.y = velocity.y - (fall_acceleration * delta)
	#print("ori: ")
	#print("dest: ")
	#print(dest)
	#print("direction: ")
	#print(direction)
	#print("target_velocity: ")
	#print(target_velocity)
	#print("")
	#shape.set_disabled(true)
	move_and_slide()
	#shape.set_disabled(false)
