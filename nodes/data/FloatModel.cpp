#include "FloatModel.h"
#include <QLineEdit>
#include <QDoubleValidator>
#include "../../tools/DataSerializeTool.h"

FloatModel::FloatModel()
    : m_lineEdit{ nullptr }
    , m_data(std::make_shared<FloatData>(0.0))
{}

FloatModel::~FloatModel()
{}

bool FloatModel::captionVisible() const
{
	return true;
}

QString FloatModel::caption() const
{
	return m_data->type().name;
}

bool FloatModel::portCaptionVisible(PortType portType, PortIndex portIndex) const
{
	return false;
}

QString FloatModel::portCaption(PortType portType, PortIndex portIndex) const
{
	return QString("portCaptionTest");
}

QString FloatModel::name() const
{
	return m_data->type().name;
}

QJsonObject FloatModel::save() const
{
	QJsonObject modelJson = NodeDelegateModel::save();
    modelJson[m_data->type().name] = QString::number(m_data->value());
	return modelJson;
}

void FloatModel::load(QJsonObject const& modelJson)
{
    QJsonValue v = modelJson[m_data->type().name];

    if (!v.isUndefined()) {
        QString strNum = v.toString();

        bool ok;
        float data = strNum.toFloat(&ok);
        if (ok) {
            m_data = std::make_shared<FloatData>(data);

            if (m_lineEdit)
                m_lineEdit->setText(strNum);
        }
    }
}

unsigned int FloatModel::nPorts(PortType portType) const
{
    unsigned int result = 1;

    switch (portType) {
    case PortType::In:
        result = 0;
        break;

    case PortType::Out:
        result = 1;

    default:
        break;
    }

    return result;
}

NodeDataType FloatModel::dataType(PortType portType, PortIndex portIndex) const
{
    return FloatData().type();
}

void FloatModel::setInData(std::shared_ptr<NodeData> nodeData, PortIndex const portIndex)
{
}

std::shared_ptr<NodeData> FloatModel::outData(PortIndex const port)
{
    return m_data;
}

QWidget* FloatModel::embeddedWidget()
{
    if (!m_lineEdit) {
        m_lineEdit = new QLineEdit();
        m_lineEdit->setValidator(new QDoubleValidator());
        m_lineEdit->setMaximumSize(m_lineEdit->sizeHint());
        m_lineEdit->setText(QString::number(m_data->value()));
        m_lineEdit->resize(100, 30);

        connect(m_lineEdit, &QLineEdit::textChanged, this, &FloatModel::onTextEdited);
    }

    return m_lineEdit;
}

void FloatModel::onTextEdited(QString const& string)
{
    bool ok = false;

    double data = string.toFloat(&ok);

    if (ok) {
        m_data = std::make_shared<FloatData>(data);

        Q_EMIT dataUpdated(0);

    }
    else {
        Q_EMIT dataInvalidated(0);
    }
}
