function vtuber_avatar_talk(top, bottom, val)
	top:relative_move(math.sin(val) * val, val)
end