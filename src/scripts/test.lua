function vtuber_avatar_talk(top, bottom, scale)
	max_height = 150
	max_width = 50
	max_rotation = 12.5 --rotation in degrees

	top:set_rotation_axis(0, 1)
	top:set_rotation(scale * max_rotation)
end