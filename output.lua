print("Hello Lua World!")

width = 1280
height = 800

function add(a, b)
	print("this is a function")

	--查看压入的a、b值
	print("a: " .. a)
	print("b: " .. b)

	return (a + b)
end

-- 访问C++函数
print(show())

-- 表
application = {
	name = "window",
	width = 1280, 
	height = 800
}