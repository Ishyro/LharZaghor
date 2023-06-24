extends VBoxContainer


# Called when the node enters the scene tree for the first time.
func _ready():
	$"New Game".grab_focus()

func _on_new_game_pressed():
	get_tree().change_scene_to_file("res://world/world.tscn")


func _on_load_game_pressed():
	pass # Replace with function body.


func _on_join_pressed():
	pass # Replace with function body.


func _on_settings_pressed():
	get_tree().change_scene_to_file("res://menus/settings.tscn")


func _on_back_pressed():
	get_tree().change_scene_to_file("res://menus/main_menu.tscn")
