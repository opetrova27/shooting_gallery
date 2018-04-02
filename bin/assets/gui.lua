function MediatorFunc(message)
	--WriteMessage("MediatorFunc: "..message:getPublisher().." - "..message:getData())
  if message:getPublisher() == "ChangeLayer" then
    Screen:popLayer();
    Screen:pushLayer(message:getData());
  end;
end