
function timage_main(orig_width, orig_height, req_width, req_height)
	local comp_width;
	local comp_height;

	orig_ratio = orig_width/orig_height
	req_ratio = req_width/req_height

	if orig_ratio > req_ratio then
		comp_width = req_width
		comp_height = req_height / orig_ratio
	else
		comp_width = req_width * orig_ratio
		comp_height = req_height
	end

--	print (orig_ratio)
--	print (req_ratio)
--	print (comp_width)
--	print (comp_height)

	if req_width ~= comp_width or req_height ~= comp_height then
		array = actions.new(2)
		actions.set(array, 1, '1,' .. comp_width .. ',' .. comp_height)
		actions.set(array, 2, '3,' .. req_width .. ',' .. req_height)
	else
		array = actions.new(1)
		actions.set(array, 1, '1,' .. comp_width .. ',' .. comp_height)
	end

	return array
end

-- timage_main(1024, 768, 100, 100)

