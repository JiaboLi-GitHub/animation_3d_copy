#include "CubeModel.h"
#include "tools/DataSerializeTool.h"
#include "../NodeData.h"

CubeModel::CubeModel()
	: m_position(std::make_shared<Vec3Data>())
	, m_scaleSize(std::make_shared<Vec3Data>())
	, m_color(std::make_shared<Vec3Data>())
{}

CubeModel::~CubeModel()
{}

bool CubeModel::captionVisible() const
{
	return true;
}

QString CubeModel::caption() const
{
	return QStringLiteral("Cube");
}

bool CubeModel::portCaptionVisible(PortType portType, PortIndex portIndex) const
{
	return true;
}

QString CubeModel::portCaption(PortType portType, PortIndex portIndex) const
{
	if (portType == PortType::In)
	{
		if (portIndex == 0)
			return QStringLiteral("position");
		if (portIndex == 1)
			return QStringLiteral("scaleSize");
		if (portIndex == 2)
			return QStringLiteral("color");
	}

	return QString();
}

QString CubeModel::name() const
{
	return QStringLiteral("Cube");
}

QJsonObject CubeModel::save() const
{
	QJsonObject modelJson = NodeDelegateModel::save();
	modelJson["position"] = DataSerializeTool::toString(m_position->value());
	modelJson["scaleSize"] = DataSerializeTool::toString(m_scaleSize->value());
	modelJson["color"] = DataSerializeTool::toString(m_color->value());
	return modelJson;
}

void CubeModel::load(QJsonObject const& modelJson)
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

	readVec3Fun(modelJson["position"], m_position.get());
	readVec3Fun(modelJson["scaleSize"], m_scaleSize.get());
	readVec3Fun(modelJson["color"], m_color.get());
}

unsigned int CubeModel::nPorts(PortType portType) const
{
	unsigned int result = 1;

	switch (portType) {
	case PortType::In:
		result = 3;
		break;
	case PortType::Out:
		result = 0;
	default:
		break;
	}

	return result;
}

NodeDataType CubeModel::dataType(PortType portType, PortIndex portIndex) const
{
	if (portType == PortType::In)
	{
		if (portIndex <= 2)
			return Vec3Data().type();
	}

	return NodeDataType();
}

void CubeModel::setInData(std::shared_ptr<NodeData> nodeData, PortIndex const portIndex)
{
	if (portIndex == 0)
	{
		auto vec3Data = std::dynamic_pointer_cast<Vec3Data>(nodeData);

		if (vec3Data)
			m_position = vec3Data;
		else
			Q_EMIT dataInvalidated(portIndex);
	}
	else if (portIndex == 1)
	{
		auto vec3Data = std::dynamic_pointer_cast<Vec3Data>(nodeData);

		if (vec3Data)
			m_scaleSize = vec3Data;
		else
			Q_EMIT dataInvalidated(portIndex);
	}
	else if (portIndex == 2)
	{
		auto vec3Data = std::dynamic_pointer_cast<Vec3Data>(nodeData);

		if (vec3Data)
			m_color = vec3Data;
		else
			Q_EMIT dataInvalidated(portIndex);
	}
}

std::shared_ptr<NodeData> CubeModel::outData(PortIndex const port)
{
	return std::shared_ptr<NodeData>();
}

QWidget* CubeModel::embeddedWidget()
{
	return nullptr;
}