-- set background image
bg("草地森林")

-- add armature role
arm('guanyu', 'guanyu', 100, 500)
armplay('guanyu', '待机')

-- set default value for name and messsage
msgsize(40)
msgcolor(0xfff33290)
namepos(480, 800)
namesize(60)
namecolor(0xff008923)

-- show message
msg('Me', '我的我的我的我的我的\n我的我的我的我的\n我的我的我的我的\n我的我的我的我的\n我的我的我的我的我的')
msg('You', 'hello world! haha\nwhat is your name?')

-- wait for 2 seconds
wait(2)

-- shake guanyu
move('guanyu', 400, 500, 1)
wait(1)
armplay('guanyu', '进攻')
waitarm()
armplay('guanyu', '待机')

-- add a role and set it 45 degree
img('weapon', "关羽_武器.png", 100, 200)
angle('weapon', 45)