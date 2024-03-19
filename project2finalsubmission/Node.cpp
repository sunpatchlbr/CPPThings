/*// Luke Ravsten LBR210000 CS2337.002
Node::Node()
{
   dataObject = nullptr;
   }

template <typename T>
Node::Node(T inputObject)
{
   dataObject = &inputObject;
   }

template <typename T>
T Node::getData()
{
   return *dataObject;
   }*/