#include "DirectionalLight.h"
#include "tools/DataSerializeTool.h"
#include "../NodeData.h"

DirectionalLight::DirectionalLight()
	: m_direction(std::make_shared<Vec3Data>())
	, m_color(std::make_shared<Vec3Data>())
{}

DirectionalLight::~DirectionalLight()
{}

bool DirectionalLight::captionVisible() const
{
	return true;
}

QString DirectionalLight::caption() const
{
	return QStringLiteral("DirectionalLight");
}

bool DirectionalLight::portCaptionVisible(PortType portType, PortIndex portIndex) const
{
	return true;
}

QString DirectionalLight::portCaption(PortType portType, PortIndex portIndex) const
{
	if (portType == PortType::In)
	{
		if (portIndex == 0)
			return QStringLiteral("direction");
		if (portIndex == 1)
			return QStringLiteral("color");
	}

	return QString();
}

QString DirectionalLight::name() const
{
	return QStringLiteral("DirectionalLight");
}

QJsonObject DirectionalLight::save() const
{
	QJsonObject modelJson = NodeDelegateModel::save();
	modelJson["direction"] = DataSerializeTool::toString(m_direction->value());
	modelJson["color"] = DataSerializeTool::toString(m_color->value());
	return modelJson;
}

void DirectionalLight::load(QJsonObject const& modelJson)
{
	auto readVec3Fun = [](const QJsonValue& v, Vec3Data* vec3Data)
		{
			if (!v.isUndefined())
			{
				QString str = v.toString();

				bool ok;
				auto data = DataSerializeTool::toVec3(str, &ok);
				if (ok)
					vec3Data->setValue(data);
			}
		};

	readVec3Fun(modelJson["direction"], m_direction.get());
	readVec3Fun(modelJson["color"], m_color.get());
}

unsigned int DirectionalLight::nPorts(PortType portType) const
{
	unsigned int result = 1;

	switch (portType) {
	case PortType::In:
		result = 2;
		break;

	case PortType::Out:
		result = 0;

	default:
		break;
	}

	return result;
}

NodeDataType DirectionalLight::dataType(PortType portType, PortIndex portIndex) const
{
	if (portType == PortType::In)
	{
		if (portIndex <= 1)
			return Vec3Data().type();
	}

	return NodeDataType();
}

void DirectionalLight::setInData(std::shared_ptr<NodeData> nodeData, PortIndex const portIndex)
{
	if (portIndex == 0)
	{
		auto vec3Data = std::dynamic_pointer_cast<Vec3Data>(nodeData);

		if (vec3Data)
			m_direction = vec3Data;
		else
			Q_EMIT dataInvalidated(portIndex);
	}
	else if (portIndex == 1)
	{
		auto vec3Data = std::dynamic_pointer_cast<Vec3Data>(nodeData);

		if (vec3Data)
			m_color = vec3Data;
		else
			Q_EMIT dataInvalidated(portIndex);
	}
}

std::shared_ptr<NodeData> DirectionalLight::outData(PortIndex const port)
{
	return std::shared_ptr<NodeData>();
}

QWidget* DirectionalLight::embeddedWidget()
{
	return nullptr;
}
