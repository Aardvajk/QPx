#ifndef QPX_PROPERTY_BROWSER_MODEL_H
#define QPX_PROPERTY_BROWSER_MODEL_H

#include <QPxCore/QPxTreeModel.h>

class qpx_property_browser_model : public QPx::TreeModel
{
    Q_OBJECT

public:
    explicit qpx_property_browser_model(QObject *parent);

    virtual QVariant data(const QModelIndex &index, int role) const override;

    virtual int columnCount(const QModelIndex &parent) const override;
};

#endif // QPX_PROPERTY_BROWSER_MODEL_H
