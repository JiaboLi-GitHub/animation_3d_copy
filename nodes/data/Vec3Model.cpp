#include "Vec3Model.h"
#include <QLineEdit>
#include <QDoubleValidator>
#include <QVBoxLayout>
#include <QLabel>
#include "tools/DataSerializeTool.h"
#include "../NodeData.h"

Vec3Model::Vec3Model()
	: m_widget{ nullptr }
	, m_lineEditX{ nullptr }
	, m_lineEditY{ nullptr }
	, m_lineEditZ{ nullptr }
	, m_data(std::make_shared<Vec3Data>())
{}

Vec3Model::~Vec3Model()
{}

bool Vec3Model::captionVisible() const
{
	return true;
}

QString Vec3Model::caption() const
{
	return m_data->type().name;
}

bool Vec3Model::portCaptionVisible(PortType portType, PortIndex portIndex) const
{
	return false;
}

QString Vec3Model::portCaption(PortType portType, PortIndex portIndex) const
{
	return QString();
}

QString Vec3Model::name() const
{
	return m_data->type().name;
}

QJsonObject Vec3Model::save() const
{
	QJsonObject modelJson = NodeDelegateModel::save();
	modelJson[m_data->type().name] = DataSerializeTool::toString(m_data->value());
	return modelJson;
}

void Vec3Model::load(QJsonObject const& modelJson)
{
	QJsonValue v = modelJson[m_data->type().name];

	if (!v.isUndefined()) {
		QString str = v.toString();

		bool ok;
		auto data = DataSerializeTool::toVec3(str, &ok);
		if (ok) {
			m_data = std::make_shared<Vec3Data>(data);

			if (m_lineEditX)
				m_lineEditX->setText(QString::number(data.x));
			if (m_lineEditY)
				m_lineEditY->setText(QString::number(data.y));
			if (m_lineEditZ)
				m_lineEditZ->setText(QString::number(data.z));
		}
	}
}

unsigned int Vec3Model::nPorts(PortType portType) const
{
	unsigned int result = 1;

	switch (portType) {
	case PortType::In:
		result = 3;
		break;

	case PortType::Out:
		result = 1;

	default:
		break;
	}

	return result;
}

NodeDataType Vec3Model::dataType(PortType portType, PortIndex portIndex) const
{
	if (portType == PortType::In)
		return FloatData().type();
	return Vec3Data().type();
}

void Vec3Model::setInData(std::shared_ptr<NodeData> nodeData, PortIndex const portIndex)
{
	if (portIndex == 0)
	{
		auto floatData = std::dynamic_pointer_cast<FloatData>(nodeData);

		if (floatData)
		{
			m_data->setX(floatData->value());
			if(m_lineEditX)
				m_lineEditX->setText(QString::number(floatData->value()));
		}
		else
		{
			Q_EMIT dataInvalidated(portIndex);
		}
	}
	else if (portIndex == 1)
	{
		auto floatData = std::dynamic_pointer_cast<FloatData>(nodeData);

		if (floatData)
		{
			m_data->setY(floatData->value());
			if(m_lineEditY)
				m_lineEditY->setText(QString::number(floatData->value()));
		}
		else
		{
			Q_EMIT dataInvalidated(portIndex);
		}
	}
	else if (portIndex == 2)
	{
		auto floatData = std::dynamic_pointer_cast<FloatData>(nodeData);

		if (floatData)
		{
			m_data->setZ(floatData->value());
			if(m_lineEditZ)
				m_lineEditZ->setText(QString::number(floatData->value()));
		}
		else
		{
			Q_EMIT dataInvalidated(portIndex);
		}
	}
}

std::shared_ptr<NodeData> Vec3Model::outData(PortIndex const port)
{
	return m_data;
}

QWidget* Vec3Model::embeddedWidget()
{
	if (!m_widget)
	{
		m_widget = new QWidget();
		m_widget->setAttribute(Qt::WA_TranslucentBackground);
		m_widget->setObjectName(QStringLiteral("NodeWidget"));
		QVBoxLayout* layout = new QVBoxLayout;
		layout->setContentsMargins({ 0,0,0,0 });

		QHBoxLayout* hLayoutX = new QHBoxLayout;
		QLabel* labelX = new QLabel(QStringLiteral("X："));
		m_lineEditX = new QLineEdit(m_widget);
		hLayoutX->addWidget(labelX);
		hLayoutX->addWidget(m_lineEditX);
		layout->addLayout(hLayoutX);
		m_lineEditX->setValidator(new QDoubleValidator());
		m_lineEditX->setText(QString::number(m_data->value().x));
		connect(m_lineEditX, &QLineEdit::textChanged, this, &Vec3Model::slotTextEdited);

		QHBoxLayout* hLayoutY = new QHBoxLayout;
		QLabel* labelY = new QLabel(QStringLiteral("Y："));
		m_lineEditY = new QLineEdit(m_widget);
		hLayoutY->addWidget(labelY);
		hLayoutY->addWidget(m_lineEditY);
		layout->addLayout(hLayoutY);
		m_lineEditY->setValidator(new QDoubleValidator());
		m_lineEditY->setText(QString::number(m_data->value().y));
		connect(m_lineEditY, &QLineEdit::textChanged, this, &Vec3Model::slotTextEdited);

		QHBoxLayout* hLayoutZ = new QHBoxLayout;
		QLabel* labelZ = new QLabel(QStringLiteral("Z："));
		m_lineEditZ = new QLineEdit(m_widget);
		hLayoutZ->addWidget(labelZ);
		hLayoutZ->addWidget(m_lineEditZ);
		layout->addLayout(hLayoutZ);
		m_lineEditZ->setValidator(new QDoubleValidator());
		m_lineEditZ->setText(QString::number(m_data->value().z));
		connect(m_lineEditZ, &QLineEdit::textChanged, this, &Vec3Model::slotTextEdited);

		m_widget->setLayout(layout);
		m_widget->resize(100, 100);
	}
	return m_widget;
}

void Vec3Model::slotTextEdited(QString const& string)
{
	QLineEdit* lineEdit = qobject_cast<QLineEdit*>(sender());
	if (!lineEdit)
		return;

	int number = -1;
	if (m_lineEditX == lineEdit)
		number = 0;
	else if (m_lineEditY == lineEdit)
		number = 1;
	else if (m_lineEditZ == lineEdit)
		number = 2;
	if (number == -1)
		return;

	bool ok = false;
	float value = string.toFloat(&ok);

	if (ok) {
		if (number == 0)
			m_data->setX(value);
		else if (number == 1)
			m_data->setY(value);
		else
			m_data->setZ(value);

		lineEdit->setText(QString::number(value));
		Q_EMIT dataUpdated(number);
	}
	else {
		Q_EMIT dataInvalidated(number);
	}
}
